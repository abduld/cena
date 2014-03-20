

#include "parser.h"

#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/Signals.h"

#include <string>
#include <vector>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


Rewriter rewriter;




class ExampleVisitor : public RecursiveASTVisitor<ExampleVisitor> {
private:
	clang::ASTContext *astContext; // used for getting additional AST info

public:
	explicit ExampleVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
	}


	bool VisitTypedefDecl(const TypedefDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitTypeAliasDecl(const TypeAliasDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitEnumDecl(const EnumDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitEnumConstantDecl(const EnumConstantDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitFunctionDecl(const FunctionDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitFieldDecl(const FieldDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitVarDecl(const VarDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitImplicitParamDecl(const ImplicitParamDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitBlockDecl(const BlockDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitCapturedDecl(const CapturedDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitLabelDecl(const LabelDecl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitDecl(const Decl * decl) {
		decl->dumpColor();
		return true;
	}

	bool VisitStmt(const Stmt * stmt) {
		stmt->dumpColor();
		return true;
	}

};


class ExampleASTConsumer : public ASTConsumer {
private:
	ExampleVisitor *visitor; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit ExampleASTConsumer(CompilerInstance *CI)
		: visitor(new ExampleVisitor(CI)) // initialize the visitor
	{ }

	virtual void HandleTranslationUnit(ASTContext &Ctx) {
		visitor->TraverseDecl(Ctx.getTranslationUnitDecl());
	}

	// override this to call our ExampleVisitor on each top-level Decl
	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
		// a DeclGroupRef may have multiple Decls, so we iterate through each one
		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
			Decl *D = *i;
			visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
		}
		return true;
	}
};

class ExampleFrontendAction : public ASTFrontendAction {
public:
	virtual ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
		return new ExampleASTConsumer(&CI); // pass CI pointer to ASTConsumer
	}
};

void parse() {
	llvm::sys::PrintStackTraceOnErrorSignal();

	std::unique_ptr<CompilationDatabase> Compilations(new FixedCompilationDatabase("/", vector<string>()));
	vector<string> Sources;
	Sources.push_back("main.cpp");

	ClangTool Tool(*Compilations, Sources);
	
	Tool.mapVirtualFile("/a.cc", "int x = 3; void do_math(int *x) {*x += 5;}");

	vector<ASTUnit *> ASTs;
	Tool.buildASTs(ASTs);

	int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>());

	// print out the rewritten source code ("rewriter" is a global var.)
	rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

	llvm::DeleteContainerPointers(ASTs);
}