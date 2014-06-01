

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
#include "SymbolicForm.h"

#include <string>
#include <vector>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


Rewriter rewriter;


#define DEBUG printf("DEBUG :: >>> %d ... \n", __LINE__)


// All unary operators.
#define UNARYOP_LIST()                                                         \
OPERATOR(PostInc) OPERATOR(PostDec) OPERATOR(PreInc) OPERATOR(PreDec)        \
OPERATOR(AddrOf) OPERATOR(Deref) OPERATOR(Plus) OPERATOR(Minus)          \
OPERATOR(Not) OPERATOR(LNot) OPERATOR(Real) OPERATOR(Imag)               \
OPERATOR(Extension)

// All binary operators (excluding compound assign operators).
#define BINOP_LIST()                                                           \
OPERATOR(PtrMemD) OPERATOR(PtrMemI) OPERATOR(Mul) OPERATOR(Div)              \
OPERATOR(Rem) OPERATOR(Add) OPERATOR(Sub) OPERATOR(Shl) OPERATOR(Shr)    \
OPERATOR(LT) OPERATOR(GT) OPERATOR(LE) OPERATOR(GE) OPERATOR(EQ)         \
OPERATOR(NE) OPERATOR(And) OPERATOR(Xor) OPERATOR(Or) OPERATOR(LAnd)     \
OPERATOR(LOr) OPERATOR(Assign) OPERATOR(Comma)

// All compound assign operators.
#define CAO_LIST()                                                             \
OPERATOR(Mul) OPERATOR(Div) OPERATOR(Rem) OPERATOR(Add) OPERATOR(Sub)        \
OPERATOR(Shl) OPERATOR(Shr) OPERATOR(And) OPERATOR(Or) OPERATOR(Xor)

class MVisitor : public RecursiveASTVisitor<MVisitor> {
private:
	clang::ASTContext *astContext; // used for getting additional AST info

public:
	explicit MVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
	}


	bool VisitTypedefDecl(const TypedefDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitTypeAliasDecl(const TypeAliasDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitEnumDecl(const EnumDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitEnumConstantDecl(const EnumConstantDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitFunctionDecl(const FunctionDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitFieldDecl(const FieldDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitVarDecl(const VarDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitImplicitParamDecl(const ImplicitParamDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitBlockDecl(const BlockDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitCapturedDecl(const CapturedDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitLabelDecl(const LabelDecl * decl) {
		DEBUG;
		decl->dump();
		return true;
	}

	bool VisitDecl(Decl * decl) {
		if (isa<FunctionDecl>(decl)) {
			return VisitFunctionDecl(cast<FunctionDecl>(decl));
		} else if (isa<TranslationUnitDecl>(decl)) {
			TranslationUnitDecl * tud = cast<TranslationUnitDecl>(decl);
			for(TranslationUnitDecl::decl_iterator I = tud->decls_begin(), E = tud->decls_end();
				I != E; ++I) {
				TraverseDecl(*I);
			}
		}
		return true;
	}
    
	bool VisitDeclStmt(DeclStmt * decl) {
		DEBUG;
		decl->dump();
		return true;
	}
    
	bool VisitIfStmt(IfStmt * ifStmt) {
		DEBUG;
        Expr *conditionExpr = ifStmt->getCond();
        Stmt *thenStmt = ifStmt->getThen();
        Stmt *elseStmt = ifStmt->getElse();
        VisitExpr(conditionExpr);
        VisitStmt(thenStmt);
        if (elseStmt != NULL) {
            VisitStmt(elseStmt);
        }
		ifStmt->dump();
		return true;
	}

    bool VisitFunctionDecl(FunctionDecl *f) {
		if (f->hasBody()) {
            Stmt * body = f->getBody();
            VisitStmt(body);
		}
		return true;
    }
    
    bool VisitCompoundStmt(CompoundStmt *stmt) {
		DEBUG;
		stmt->dump();
        for (CompoundStmt::const_body_iterator citer = stmt->body_begin();
             citer != stmt->body_end();
             ++citer) {
            VisitStmt(*citer);
        }
		return true;
    }
    
    bool VisitIntegerLiteral(IntegerLiteral *L) {
		DEBUG;
		L->dump();
        if (L->getType()->isUnsignedIntegerType()) {
            std::clog << "TODO;;;" << std::endl;
        } else if (L->getValue().getNumWords() == 1) {
            std::clog << " Signed " << L->getValue().getSExtValue() << std::endl;
        } else {
            std::clog << "TODO;;;" << std::endl;
        }
		return true;
    }

	bool VisitExpr(Expr * expr) {
		DEBUG;
		expr->dump();
#define VISIT(type) do {                                                \
clang::type* concrete_expr = dyn_cast_or_null<clang::type>(expr); \
if (concrete_expr != NULL) {                                      \
return Visit##type (concrete_expr);                        \
}                                                                 \
} while(0);
        
        //VISIT(AbstractConditionalOperator);
        //VISIT(AddrLabelExpr);
        //VISIT(ArraySubscriptExpr);
        //VISIT(BinaryOperator);
        //VISIT(BinaryTypeTraitExpr);
        //VISIT(BlockDeclRefExpr);
        //VISIT(BlockExpr);
        //VISIT(CallExpr);
        //VISIT(CastExpr);
        //VISIT(CharacterLiteral);
        //VISIT(ChooseExpr);
        //VISIT(CompoundLiteralExpr);
        //VISIT(CXXBindTemporaryExpr);
        //VISIT(CXXBoolLiteralExpr);
        //VISIT(CXXConstructExpr);
        //VISIT(CXXDefaultArgExpr);
        //VISIT(CXXDeleteExpr);
        //VISIT(CXXDependentScopeMemberExpr);
        //VISIT(CXXNewExpr);
        //VISIT(CXXNoexceptExpr);
        //VISIT(CXXNullPtrLiteralExpr);
        //VISIT(CXXPseudoDestructorExpr);
        //VISIT(CXXScalarValueInitExpr);
        //VISIT(CXXThisExpr);
        //VISIT(CXXThrowExpr);
        //VISIT(CXXTypeidExpr);
        //VISIT(CXXUnresolvedConstructExpr);
        //VISIT(CXXUuidofExpr);
        //VISIT(DeclRefExpr);
        //VISIT(DependentScopeDeclRefExpr);
        //VISIT(DesignatedInitExpr);
        //VISIT(ExprWithCleanups);
        //VISIT(ExtVectorElementExpr);
        //VISIT(FloatingLiteral);
        //VISIT(GNUNullExpr);
        //VISIT(ImaginaryLiteral);
        //VISIT(ImplicitValueInitExpr);
        //VISIT(InitListExpr);
        VISIT(IntegerLiteral);
        //VISIT(MemberExpr);
        //VISIT(OffsetOfExpr);
        //VISIT(OpaqueValueExpr);
        //VISIT(OverloadExpr);
        //VISIT(PackExpansionExpr);
        //VISIT(ParenExpr);
        //VISIT(ParenListExpr);
        //VISIT(PredefinedExpr);
        //VISIT(ShuffleVectorExpr);
        //VISIT(SizeOfPackExpr);
        //VISIT(StmtExpr);
        //VISIT(StringLiteral);
        //VISIT(SubstNonTypeTemplateParmPackExpr);
        //VISIT(UnaryExprOrTypeTraitExpr);
        //VISIT(UnaryOperator);
        //VISIT(UnaryTypeTraitExpr);
        //VISIT(VAArgExpr);
#undef VISIT
    
		return true;
	}

	bool VisitStmt(Stmt * stmt) {
#define VISIT(type) do {                                                \
clang::type* concrete_stmt = dyn_cast_or_null<clang::type>(stmt); \
if (concrete_stmt != NULL) {                                      \
return Visit##type (concrete_stmt);                        \
}                                                                 \
} while(0);
        DEBUG;
		stmt->dump();
        
        //VISIT(AsmStmt);
        //VISIT(BreakStmt);
        VISIT(CompoundStmt);
        //VISIT(ContinueStmt);
        //VISIT(CXXCatchStmt);
        //VISIT(CXXTryStmt);
        VISIT(DeclStmt);
        //VISIT(DoStmt);
        //VISIT(ForStmt);
        //VISIT(GotoStmt);
        VISIT(IfStmt);
        //VISIT(IndirectGotoStmt);
        //VISIT(LabelStmt);
        //VISIT(NullStmt);
        //VISIT(ReturnStmt);
        //VISIT(CaseStmt);
        //VISIT(DefaultStmt);
        //VISIT(SwitchStmt);
        //VISIT(WhileStmt);
#undef VISIT

		return true;
	}

};


class JSONASTConsumer : public ASTConsumer {
private:
	MVisitor *visitor;
public:
	explicit JSONASTConsumer(CompilerInstance *CI)
		: visitor(new MVisitor(CI))
	{ }

    /*
	virtual void HandleTranslationUnit(ASTContext &Ctx) {
		visitor->TraverseDecl(Ctx.getTranslationUnitDecl());
	}
     */

	// override this to call our MVisitor on each top-level Decl
	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
		// a DeclGroupRef may have multiple Decls, so we iterate through each one
		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
			Decl *D = *i;
			visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
		}
		return true;
	}
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
	//Sources.push_back("test.cpp");

	//ClangTool Tool(*Compilations, Sources);
	
	//Tool.mapVirtualFile("/a.cc", "int x = 3; void do_math(int *x) {*x += 5;}");

	//vector<std::unique_ptr<ASTUnit>> ASTs;
	//Tool.buildASTs(ASTs);
    
    //Tool.run(newFrontendActionFactory<JSONFrontendAction>().get());

    
    vector<string> args;
    args.push_back("-std=c99");
    
    runToolOnCodeWithArgs(newFrontendActionFactory<JSONFrontendAction>()->create(),
                  "int main() { return 1; }",
                          args
                  );
	// print out the rewritten source code ("rewriter" is a global var.)
	// rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

	//llvm::DeleteContainerPointers(ASTs);
}