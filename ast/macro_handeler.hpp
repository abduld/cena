
#ifndef __MACRO_HANDLER_H__
#define __MACRO_HANDLER_H__

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
#include "node/ast.hpp"

#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace llvm;
using namespace clang;

class PreprocessorCallback : public PPCallbacks {
  Preprocessor &PP;
  bool disabled = false; // To prevent recurstion

public:
  PreprocessorCallback(Preprocessor &PP);
  ~PreprocessorCallback();

  void MacroExpands(const Token &MacroNameTok, const MacroInfo *MI,
                    SourceRange Range);

  void MacroExpands(const Token &MacroNameTok, const MacroDirective *MD,
                    SourceRange Range, const MacroArgs *Args);

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          llvm::StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange, const FileEntry *File,
                          llvm::StringRef SearchPath,
                          llvm::StringRef RelativePath, const Module *Imported);
  void If(SourceLocation Loc, SourceRange ConditionRange,
          ConditionValueKind ConditionValue);
  void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
              const MacroDirective *MD);
  void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
             const MacroDirective *MD);
  void Elif(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue, SourceLocation IfLoc);
  void Else(SourceLocation Loc, SourceLocation IfLoc);
  void Endif(SourceLocation Loc, SourceLocation IfLoc);
  bool FileNotFound(llvm::StringRef FileName,
                    llvm::SmallVectorImpl<char> &RecoveryPath);

private:
  std::map<SourceLocation, SourceLocation> ElifMapping; // Map an elif location
                                                        // to the real if;
  void HandlePPCond(SourceLocation Loc, SourceLocation IfLoc) {}
};

#endif /* __MACRO_HANDLER_H__ */
