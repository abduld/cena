

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
#include "jansson.h"

#include <string>
#include <vector>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


Rewriter rewriter;


#define DEBUG printf("DEBUG :: >>> %d ... \n", __LINE__)


class JSONVisitor : public RecursiveASTVisitor<JSONVisitor> {
private:
	clang::ASTContext *astContext; // used for getting additional AST info

public:
	explicit JSONVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
	}


	json_t * VisitTypedefDecl(const TypedefDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitTypeAliasDecl(const TypeAliasDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitEnumDecl(const EnumDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitEnumConstantDecl(const EnumConstantDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitFunctionDecl(const FunctionDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitFieldDecl(const FieldDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitVarDecl(const VarDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitImplicitParamDecl(const ImplicitParamDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitBlockDecl(const BlockDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitCapturedDecl(const CapturedDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitLabelDecl(const LabelDecl * decl) {
		DEBUG;
		decl->dumpColor();
		return NULL;
	}

	json_t * VisitDecl(Decl * decl) {
		if (isa<FunctionDecl>(decl)) {
			return VisitFunctionDecl(cast<FunctionDecl>(decl));
		} else if (isa<TranslationUnitDecl>(decl)) {
			TranslationUnitDecl * tud = cast<TranslationUnitDecl>(decl);
			for(TranslationUnitDecl::decl_iterator I = tud->decls_begin(), E = tud->decls_end();
				I != E; ++I) {
				TraverseDecl(*I);
			}
		}
		return NULL;
	}

    json_t * VisitFunctionDecl(FunctionDecl *f) {
		if (f->hasBody()) {
            Stmt * body = f->getBody();
            VisitStmt(body);
		}
		return NULL;
    }

	json_t * VisitExpr(Expr * expr) {
		DEBUG;
		expr->dumpColor();
		return NULL;
	}

	json_t * VisitStmt(Stmt * stmt) {
		printf("Stmt ... \n");
		if (isa<CompoundStmt>(stmt)) {
			CompoundStmt * st = cast<CompoundStmt>(stmt);
			for(CompoundStmt::body_iterator I = st->body_begin(), E = st->body_end();
				I != E; ++I) {
				VisitStmt(*I);
			}
			return NULL;
		} else if (isa<Expr>(stmt)) {
			return VisitExpr(cast<Expr>(stmt));
		}
		stmt->dumpColor();
		return NULL;
	}

};


class JSONASTConsumer : public ASTConsumer {
private:
	JSONVisitor *visitor;
public:
	explicit JSONASTConsumer(CompilerInstance *CI)
		: visitor(new JSONVisitor(CI))
	{ }

	virtual void HandleTranslationUnit(ASTContext &Ctx) {
		visitor->TraverseDecl(Ctx.getTranslationUnitDecl());
	}

	// override this to call our JSONVisitor on each top-level Decl
	/*
	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
		// a DeclGroupRef may have multiple Decls, so we iterate through each one
		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
			Decl *D = *i;
			visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
		}
		return true;
	}
	*/
};

class JSONFrontendAction : public ASTFrontendAction {
public:
	virtual ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
		return new JSONASTConsumer(&CI); // pass CI pointer to ASTConsumer
	}
};

void parse() {
	llvm::sys::PrintStackTraceOnErrorSignal();

  	IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOpts(new clang::DiagnosticOptions());
	std::unique_ptr<clang::DiagnosticsEngine> diagnostics(new clang::DiagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()), &*diagnosticOpts, new clang::IgnoringDiagConsumer(), true));


	std::unique_ptr<CompilationDatabase> Compilations(new FixedCompilationDatabase("/", vector<string>()));
	vector<string> Sources;
	Sources.push_back("test.cpp");

	ClangTool Tool(*Compilations, Sources);
	
	//Tool.mapVirtualFile("/a.cc", "int x = 3; void do_math(int *x) {*x += 5;}");

	vector<ASTUnit *> ASTs;
	Tool.buildASTs(ASTs);

	int result = Tool.run(newFrontendActionFactory<JSONFrontendAction>());

	// print out the rewritten source code ("rewriter" is a global var.)
	// rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

	llvm::DeleteContainerPointers(ASTs);
}