

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
#include <boost/variant.hpp>
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

class MVisitor;
class SymbolicStmt;


typedef boost::variant<bool, int64_t, double, string> SymbolicLiteral;

typedef std::vector<SymbolicLiteral> SymbolicLiterals;

class SymbolicExpr {
public:
    SymbolicExpr(MVisitor* owner, const SymbolicLiteral & val);
    SymbolicExpr(MVisitor* owner, const SymbolicLiterals & val);
    ~SymbolicExpr();
    boost::variant<SymbolicLiteral, SymbolicLiterals> getValue() const { return val_; };
    SymbolicExpr * operator<<(SymbolicLiteral & val) {
        val_.push_back(val);
        len_++;
        return this;
    }
    SymbolicExpr * operator<<(SymbolicLiterals & vals) {
        for (auto iter = vals.begin(); iter != vals.end(); iter++) {
            val_.push_back(*iter);
        }
        len_ += vals.size();
        return this;
    }
private:
    int len_;
    std::vector<SymbolicLiteral> val_;
    SymbolicExpr * parent_;
    MVisitor* owner_;
};

typedef std::vector<SymbolicExpr> SymbolicExprs;

typedef std::vector<SymbolicStmt> SymbolicStmts;

class SymbolicStmt {
public:
    SymbolicStmt(MVisitor* owner, string head);
    ~SymbolicStmt();
    string getHead() const { return head_; }
    SymbolicExprs getArgs() const { return args_; }
    
private:
    const string head_;
    SymbolicExprs args_;
    SymbolicStmt * parent_;
    MVisitor* owner_;
};


class SymbolicBlock {
public:
    SymbolicBlock(MVisitor* owner);
    ~SymbolicBlock();
    void operator<<(SymbolicStmt s) {
        stmts_.push_back(s);
    }
    
private:
    SymbolicBlock * parent_;
    SymbolicStmts stmts_;
    MVisitor* owner_;
};

template<typename T> struct is_vector                           : public false_type {};
template<>           struct is_vector<SymbolicStmts>           : public true_type {};

template<typename T> struct is_stmt                           : public false_type {};
template<>           struct is_stmt<SymbolicStmt>           : public true_type {};


class MVisitor : public RecursiveASTVisitor<MVisitor> {
public:
	explicit MVisitor(CompilerInstance *CI)
    : astContext(&(CI->getASTContext())), block_(NULL) // initialize private members
	{
		rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
	}
    
    SymbolicExpr* EnterExpr(SymbolicExpr* expr) {
        SymbolicExpr* parent = expr_;
        expr_ = expr;
        return parent;
    }
    
    void LeaveExpr(SymbolicExpr* expr) {
        expr_ = expr;
    }
    
    SymbolicStmt* EnterStmt(SymbolicStmt* stmt) {
        SymbolicStmt* parent = stmt_;
        stmt_ = stmt;
        return parent;
    }
    
    void LeaveStmt(SymbolicStmt* stmt) {
        stmt_ = stmt;
    }
    
    SymbolicBlock* EnterBlock(SymbolicBlock* block) {
        SymbolicBlock* parent = block_;
        block_ = block;
        return parent;
    }
    
    void LeaveBlock(SymbolicBlock* block) {
        block_ = block;
    }

    bool VisitTranslationUnitDecl(TranslationUnitDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitTypedefDecl(TypedefDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitTypeAliasDecl(TypeAliasDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitEnumDecl(EnumDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitRecordDecl(RecordDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitEnumConstantDecl(EnumConstantDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitEmptyDecl(EmptyDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitFunctionDecl(FunctionDecl *D) {
        DEBUG;
        D->dump();
        return true;
	}
    
    bool VisitFriendDecl(FriendDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitFieldDecl(FieldDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitVarDecl(VarDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitLabelDecl(LabelDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitParmVarDecl(ParmVarDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitFileScopeAsmDecl(FileScopeAsmDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitImportDecl(ImportDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitStaticAssertDecl(StaticAssertDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitNamespaceDecl(NamespaceDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitUsingDirectiveDecl(UsingDirectiveDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitNamespaceAliasDecl(NamespaceAliasDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitCXXRecordDecl(CXXRecordDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitLinkageSpecDecl(LinkageSpecDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitTemplateDecl(const TemplateDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitFunctionTemplateDecl(FunctionTemplateDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitClassTemplateDecl(ClassTemplateDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCMethodDecl(ObjCMethodDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCImplementationDecl(ObjCImplementationDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCProtocolDecl(ObjCProtocolDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCCategoryImplDecl(ObjCCategoryImplDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCCategoryDecl(ObjCCategoryDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCCompatibleAliasDecl(ObjCCompatibleAliasDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCPropertyDecl(ObjCPropertyDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitObjCPropertyImplDecl(ObjCPropertyImplDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitUnresolvedUsingTypenameDecl(UnresolvedUsingTypenameDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitUnresolvedUsingValueDecl(UnresolvedUsingValueDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitUsingDecl(UsingDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitUsingShadowDecl(UsingShadowDecl *D) {
		DEBUG;
		D->dump();
		return true;
	}
    
    bool VisitOMPThreadPrivateDecl(OMPThreadPrivateDecl *D) {
		DEBUG;
		D->dump();
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
    
    bool VisitReturnStmt(ReturnStmt *stmt) {
		DEBUG;
		stmt->dump();
        
		return true;
    }
    
    bool VisitCompoundStmt(CompoundStmt *stmt) {
		DEBUG;
		//stmt->dump();
        for (CompoundStmt::const_body_iterator citer = stmt->body_begin();
             citer != stmt->body_end();
             ++citer) {
            VisitStmt(*citer);
        }
		return true;
    }
    
    bool VisitForStmt(ForStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitAsmStmt(AsmStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitBreakStmt(BreakStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitContinueStmt(ContinueStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitCXXCatchStmt(CXXCatchStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitCXXTryStmt(CXXTryStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitDoStmt(DoStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitGotoStmt(GotoStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitIndirectGotoStmt(IndirectGotoStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitLabelStmt(LabelStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitNullStmt(NullStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitDefaultStmt(DefaultStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitSwitchStmt(SwitchStmt * stmt) {
        DEBUG;
        stmt->dump();
        return true;
    }
    
    bool VisitWhileStmt(WhileStmt * stmt) {
        DEBUG;
        stmt->dump();
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
    
#if 0
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
#endif

private:
clang::ASTContext *astContext; // used for getting additional AST info
SymbolicBlock * block_;
SymbolicStmt * stmt_;
SymbolicExpr * expr_;
};


SymbolicExpr::SymbolicExpr(MVisitor* owner, const SymbolicLiteral & val) : len_(1) {
    val_.push_back(val);
    parent_ = owner_->EnterExpr(this);
}
SymbolicExpr::SymbolicExpr(MVisitor* owner, const SymbolicLiterals & val) : len_(val.size()), val_(val) {
    parent_ = owner_->EnterExpr(this);
}

SymbolicExpr::~SymbolicExpr() {
    owner_->LeaveExpr(parent_);
}

SymbolicStmt::SymbolicStmt(MVisitor* owner, string head) : head_(head), owner_(owner) {
    parent_ = owner_->EnterStmt(this);
}

SymbolicStmt::~SymbolicStmt() {
    owner_->LeaveStmt(parent_);
}

SymbolicBlock::SymbolicBlock(MVisitor* owner) : owner_(owner) {
    parent_ = owner_->EnterBlock(this);
}

SymbolicBlock::~SymbolicBlock() {
    owner_->LeaveBlock(parent_);
}

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
    std::vector<string> Sources;
	//Sources.push_back("test.cpp");

	//ClangTool Tool(*Compilations, Sources);
	
	//Tool.mapVirtualFile("/a.cc", "int x = 3; void do_math(int *x) {*x += 5;}");

	//vector<std::unique_ptr<ASTUnit>> ASTs;
	//Tool.buildASTs(ASTs);
    
    //Tool.run(newFrontendActionFactory<JSONFrontendAction>().get());

    
    std::vector<string> args;
    args.push_back("-std=c++11");
    
    runToolOnCodeWithArgs(newFrontendActionFactory<JSONFrontendAction>()->create(),
                  "int g = 3; int main() { return 1; }",
                          args
                  );
	// print out the rewritten source code ("rewriter" is a global var.)
	// rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

	//llvm::DeleteContainerPointers(ASTs);
}