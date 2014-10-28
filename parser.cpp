

#include "parser.h"

#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/LangOptions.h"
#include "llvm/Support/Signals.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"
#include "ast/visitor.hpp"
#include "ast/macro_handeler.hpp"
#include "external/backward_cpp/backward.hpp"

#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace comments;
using namespace driver;
using namespace tooling;
using namespace backward;

StackTrace st;
void collect_trace() { st.load_here(); }
struct SDiagnosticConsumer : DiagnosticConsumer {
  SDiagnosticConsumer() {}
  int HadRealError = 0;
  void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel,
                        const Diagnostic &Info) override {
    std::string clas;
    llvm::SmallString<1000> diag;
    Info.FormatDiagnostic(diag);

    switch (DiagLevel) {
    case DiagnosticsEngine::Fatal:
      std::cerr << "FATAL ";
    case DiagnosticsEngine::Error:
      std::cerr << "Error: " //<< locationToString(Info.getLocation(),
                // annotator.getSourceMgr())
                << ": " << diag.c_str() << std::endl;
      clas = "error";
      break;
    case DiagnosticsEngine::Warning:
      clas = "warning";
      break;
    default:
      break;
    }
    const_cast<DiagnosticsEngine *>(Info.getDiags())->Reset();
  }
};
class SASTConsumer : public ASTConsumer {
private:
  SVisitor *Visitor;
  CompilerInstance &CI;
  ASTContext &Ctx;
  FileID mainFileID;
  SourceManager &SM;

public:
  explicit SASTConsumer(CompilerInstance &CI)
      : CI(CI), Visitor(new SVisitor(CI)), Ctx(CI.getASTContext()),
        SM(CI.getASTContext().getSourceManager()) {
    // ci.getPreprocessor().enableIncrementalProcessing();
  }
  void Initialize(ASTContext &Ctx) override { mainFileID = SM.getMainFileID(); }

  // this to call our SVisitor on each top-level Decl
  void HandleTranslationUnit(ASTContext &context) override {
    DEBUG;
    // visitor->TraverseDecl(context.getTranslationUnitDecl());
    // find all C++ #include needed for the converted C++ types
    auto collectInclude =
        [&](clang::ASTContext &i_ctx, const clang::QualType &i_type) {
      auto decl = i_type->getAsCXXRecordDecl();
      if (decl != nullptr) {
        //decl->dump();
        /*
        auto loc = decl->clang::Decl::getLocStart();
          clang::PresumedLoc ploc = i_ctx.getSourceManager().getPresumedLoc( loc
        );
          if ( not ploc.isInvalid() )
        {
          this->_data.addCXXTypeIncludePath( ploc.getFilename() );
        }
        */
      }
    };

    std::cout << "Program : " << std::endl;
    std::cout << getProgram()->toCCode() << std::endl;
    return;
  }
  bool HandleTopLevelDecl(DeclGroupRef dg) override {
    if (CI.getDiagnostics().hasFatalErrorOccurred()) {
      // Reset errors: (Hack to ignore the fatal errors.)
      CI.getDiagnostics().Reset();
      // When there was fatal error, processing the warnings may cause crashes
    }
    for (auto decl : dg) {
      //(*iter).dump();
      if (SM.getFileID(SM.getExpansionLoc(decl->getLocation())) != mainFileID)
        continue;
      DEBUG;
      //decl->dumpColor();
      Visitor->TraverseDecl(decl);
      Visitor->addCurrent();
    }
    return true;
  }
  shared_ptr<ProgramNode> getProgram() { return Visitor->getProgram(); }
};

std::string GetExecutablePath(const char *Argv0) {
  // This just needs to be some symbol in the binary; C++ doesn't
  // allow taking the address of ::main however.
  void *main_addr = (void *)(intptr_t)GetExecutablePath;
  return llvm::sys::fs::getMainExecutable(Argv0, main_addr);
}
class SFrontendAction : public ASTFrontendAction {
public:
  SFrontendAction() : ASTFrontendAction() {
    // exe_path = GetExecutablePath(exe);
    // exe_path = exe;
  }
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {

    void *main_addr = (void *)(intptr_t)GetExecutablePath;
    /* http://code.woboq.org/mocng/src/main.cpp.html */
    clang::Preprocessor &PP = CI.getPreprocessor();
    // clang::MacroInfo *MI = PP.AllocateMacroInfo({});
    // MI->setIsBuiltinMacro();

    CI.getFrontendOpts().SkipFunctionBodies = false;
    // PP.enableIncrementalProcessing(true);
    // PP.SetSuppressIncludeNotFoundError(true);
    // PP.SetMacroExpansionOnlyInDirectives();
    // CI.getPreprocessorOpts().DisablePCHValidation = true;
    CI.getLangOpts().DelayedTemplateParsing = true;
    CI.getLangOpts().CUDA = true;
    CI.getLangOpts().EmitAllDecls = true;
    CI.getLangOpts().ImplicitInt = false;
    CI.getLangOpts().POSIXThreads = false;
    // enable all the extension
    CI.getLangOpts().MicrosoftExt = true;
    CI.getLangOpts().DollarIdents = true;
    CI.getLangOpts().CPlusPlus11 = true;
    CI.getLangOpts().GNUMode = true;

    HeaderSearchOptions &HSO = CI.getHeaderSearchOpts();
    HSO.UseBuiltinIncludes = true;
    HSO.UseStandardSystemIncludes = true;
    HSO.UseStandardCXXIncludes = true;

    CodeGenOptions &codeGenOpts = CI.getCodeGenOpts();
    codeGenOpts.RelaxAll = 1;
    codeGenOpts.RelocationModel = "static";
    codeGenOpts.DisableFPElim = 1;
    codeGenOpts.AsmVerbose = 1;
    codeGenOpts.CXXCtorDtorAliases = 1;
    codeGenOpts.UnwindTables = 1;
    codeGenOpts.OmitLeafFramePointer = 1;
    codeGenOpts.StackRealignment = 1;

    CI.getPreprocessor().addPPCallbacks(unique_ptr<PreprocessorCallback>(
        new PreprocessorCallback(CI.getPreprocessor())));
    // CI.getDiagnostics().setClient(new SDiagnosticConsumer(), true);

    /* does not work
        //shared_ptr<TargetOptions> pto( new TargetOptions());
        //pto->Triple = llvm::sys::getDefaultTargetTriple();
        // CI.setTarget( TargetInfo::CreateTargetInfo(CI.getDiagnostics(),
    pto));

      //
        // CI.setASTConsumer(std::move(astcons));
    //CI.createASTContext();
    //CI.createSema(clang::TU_Complete, NULL);
    */

    // Infer the builtin include path if unspecified.
    if (CI.getHeaderSearchOpts().UseBuiltinIncludes &&
        CI.getHeaderSearchOpts().ResourceDir.empty()) {
      CI.getHeaderSearchOpts().ResourceDir =
          CompilerInvocation::GetResourcesPath(exe_path.c_str(), main_addr);
    }
    HSO.AddPath(CI.getHeaderSearchOpts().ResourceDir + "/include",
                frontend::Angled, true, false);
    //   CI.getDiagnostics().setClient(new IgnoringDiagConsumer, true);
    //  CI.getDiagnostics().setIgnoreAllWarnings(true);

    astcons = std::unique_ptr<SASTConsumer>(new SASTConsumer(CI));

    return std::move(astcons); // pass CI pointer to ASTConsumer
  }
  shared_ptr<ProgramNode> getProgram() {
    prog_ = astcons->getProgram();

    return prog_;
  }

private:
  std::unique_ptr<SASTConsumer> astcons;
  shared_ptr<ProgramNode> prog_;
  string exe_path;
};

void parse(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  collect_trace();
  Printer printer;
  // printer.print(st, stdout);

  std::vector<string> args;
  args.emplace_back("-x");
  args.emplace_back("c++");
  // args.emplace_back("-v");
  args.emplace_back("-E");
  args.emplace_back("-fPIE");
  args.emplace_back("-std=c++11");
  args.emplace_back("-Ibuiltin");
  args.emplace_back("-isystem");
  args.emplace_back("/usr/include");
  args.emplace_back("-isystem");
  args.emplace_back("/usr/local/include");
  args.emplace_back("-Itest/wbHeaders");
  args.emplace_back("-Itest/intrinHeaders");
  // args.push_back(" -O0  ");
  // args.push_back("-fsyntax-only ");
  // args.push_back("-x cpp-output ");
  // args.push_back("-Xclang -ffake-address-space-map");
  /*
    args.emplace_back("-D__LP64__");
    args.emplace_back("-I/usr/include/");
    args.emplace_back("-I/builtins");
    // clang++ -E -x c++ - -v < /dev/null
    args.emplace_back("-I/usr/local/Cellar/llvm/HEAD/include");
    args.emplace_back("-I/usr/local/include");
    args.emplace_back("-I/usr/local/Cellar/llvm/HEAD/lib/clang/3.6.0/include");
    args.emplace_back("-I/Applications/Xcode.app/Contents/Developer/Platforms/"
                      "MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/"
                      "include/");
    args.emplace_back("-I/Applications/Xcode.app/Contents/Developer/Platforms/"
                      "MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/"
                      "include/c++/4.2.1");
    args.emplace_back("-fsyntax-only");
    */

  std::ifstream file;
  file.open(argv[1]);
  string prog{};
  string line;
  while (std::getline(file, line)) {
    prog += string("\n") + line;
  }

  std::cout << "input file = " << argv[1];
  std::cout << "input code = " << prog;

  runToolOnCodeWithArgs(newFrontendActionFactory<SFrontendAction>()->create(),
                        prog, args);
  // print out the rewritten source code ("rewriter" is a global var.)
  // rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

  // llvm::DeleteContainerPointers(ASTs);
}
