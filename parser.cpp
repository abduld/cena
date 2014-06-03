

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
#include "clang/Tooling/Tooling.h"
#include "clang/Basic/LangOptions.h"
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

#define DEBUG printf("DEBUG :: >>> %s %d ... \n", __PRETTY_FUNCTION__, __LINE__)


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

typedef boost::variant<bool, int64_t, double, string> SymbolicLiteral;

typedef std::vector<SymbolicLiteral> SymbolicLiterals;

class SymbolicExpr {
public:
    SymbolicExpr(MVisitor* owner);
    SymbolicExpr(MVisitor* owner, const SymbolicLiteral & lit);
    SymbolicExpr(MVisitor* owner, const SymbolicExpr & exp);
    void pop();
    SymbolicExpr * operator<<=(const bool & val) {
        vals_.push_back(SymbolicLiteral(val));
        len_++;
        return this;
    }
    SymbolicExpr & operator<<=(const int64_t & val) {
        vals_.push_back(SymbolicLiteral(val));
        len_++;
        return *this;
    }
    SymbolicExpr & operator<<=(const double & val) {
        vals_.push_back(SymbolicLiteral(val));
        len_++;
        return *this;
    }
    SymbolicExpr & operator<<=(const char * val) {
        vals_.push_back(SymbolicLiteral(string(val)));
        len_++;
        return *this;
    }
    SymbolicExpr & operator<<=(const string & val) {
        vals_.push_back(SymbolicLiteral(val));
        len_++;
        return *this;
    }
    SymbolicExpr & operator<<=(const SymbolicLiteral & val) {
        vals_.push_back(val);
        len_++;
        return *this;
    }
    SymbolicExpr & operator<<=(const SymbolicLiterals & vals) {
        for (auto iter = vals.begin(); iter != vals.end(); iter++) {
            vals_.push_back(*iter);
        }
        len_ += vals.size();
        return *this;
    }
    SymbolicExpr & operator<<=(const SymbolicExpr & e) {
        SymbolicLiterals vals = e.getValues();
        for (auto iter = vals.begin(); iter != vals.end(); iter++) {
            vals_.push_back(*iter);
        }
        len_ += vals.size();
        return *this;
    }
    template <typename T>
    void push_back(const T & v) {
        this << v;
    }
    SymbolicLiterals getValues() const {
        return vals_;
    }
    string toString() const {
        std::ostringstream o;
        for (auto iter = vals_.begin(); iter != vals_.end(); iter++) {
            o << *iter;
            o << ",";
        }
        return ToString(getHead(), "[", o.str(), "]");
    }
    virtual string getHead() const {
        return "Expr";
    }
protected:
    int len_;
    SymbolicLiterals vals_;
    SymbolicExpr * parent_;
    MVisitor* owner_;
};

#define DeclareSymbolicExpr(name, head) \
class Symbolic##name##Expr : public SymbolicExpr { \
public:\
    Symbolic##name##Expr(MVisitor * visitor) : SymbolicExpr(visitor) {}\
    Symbolic##name##Expr(MVisitor * visitor, const SymbolicLiteral & lit) : SymbolicExpr(visitor, lit) {}\
    Symbolic##name##Expr(MVisitor * visitor, const SymbolicExpr & exp) : SymbolicExpr(visitor, exp) {}\
virtual string getHead() const {\
    return #head;\
}\
}

DeclareSymbolicExpr(Qualifier, CQualifier);
DeclareSymbolicExpr(Type, CType);
DeclareSymbolicExpr(Pointer, CPointer);
DeclareSymbolicExpr(String, CString);
DeclareSymbolicExpr(Operator, COperator);
DeclareSymbolicExpr(Array, CArray);
DeclareSymbolicExpr(ArraySize, CArraySize);
DeclareSymbolicExpr(Cast, CCast);
DeclareSymbolicExpr(None, Skip);

typedef std::vector<SymbolicExpr> SymbolicExprs;

class SymbolicStmt {
public:
    SymbolicStmt(MVisitor* owner, string head);
    void pop();
    SymbolicStmt * operator<<=(const SymbolicExpr & val) {
        vals_.push_back(val);
        return this;
    }
    SymbolicStmt * operator<<=(const SymbolicExprs & vals) {
        for (auto iter = vals.begin(); iter != vals.end(); iter++) {
            vals_.push_back(*iter);
        }
        return this;
    }
    template <typename T>
    void push_back(const T & v) {
        operator<<=(v);
    }
    string getHead() const { return head_; }
    SymbolicExprs getArgs() const { return vals_; }
    string toString() const {
        std::ostringstream o;
        for (auto iter = vals_.begin(); iter != vals_.end(); iter++) {
            o << iter->toString();
            o << ",";
        }
        return ToString(getHead(), "[", o.str(), "]");
    }
protected:
    const string head_;
    SymbolicExprs vals_;
    SymbolicStmt * parent_;
    MVisitor* owner_;
};

#define DeclareSymbolicStmt(name, head) \
class Symbolic##name##Stmt : public SymbolicStmt { \
public:\
Symbolic##name##Stmt(MVisitor * visitor) : SymbolicStmt(visitor, std::string(#head)) {\
}\
}

DeclareSymbolicStmt(Assign, CAssign);

typedef std::vector<SymbolicStmt> SymbolicStmts;

class SymbolicBlock {
public:
    SymbolicBlock(MVisitor* owner);
    void pop();
    SymbolicBlock & operator<<=(const SymbolicStmt & s) {
        stmts_.push_back(s);
        return *this;
    }
    SymbolicBlock & operator<<=(const SymbolicStmts & ss) {
        for (auto iter = ss.begin(); iter != ss.end(); iter++) {
            stmts_.push_back(*iter);
        }
        return *this;
    }
    template <typename T>
    void push_back(const T & v) {
        operator<<=(v);
    }
    
protected:
    SymbolicBlock * parent_;
    SymbolicStmts stmts_;
    MVisitor* owner_;
};

class SymbolicFor : SymbolicBlock {
    SymbolicFor(MVisitor *owner, SymbolicExpr start, SymbolicExpr end, SymbolicExpr step) :
        SymbolicBlock(owner), start_(start), end_(end), step_(step) {}
    
private:
    SymbolicExpr start_, end_, step_;
};


class SymbolicProgram {
public:
    SymbolicProgram(MVisitor* owner);
    void pop();
    SymbolicProgram & operator<<(const SymbolicStmt & s) {
        prog_.push_back(s);
        return *this;
    }
    SymbolicProgram & operator<<(const SymbolicBlock & blk) {
        prog_.push_back(blk);
        return *this;
    }
    template <typename T>
    void push_back(const T & v) {
        this->operator<<(v);
    }
private:
    std::vector<boost::variant<SymbolicBlock, SymbolicStmt > > prog_;
    MVisitor* owner_;
};

template<typename T> struct is_literal                           : public false_type {};
template<>           struct is_literal<SymbolicLiteral>           : public true_type {};

template<typename T> struct is_iteratable                           : public false_type {};
template<>           struct is_iteratable<SymbolicLiterals>           : public true_type {};
template<>           struct is_iteratable<SymbolicExpr>           : public true_type {};
template<>           struct is_iteratable<SymbolicStmts>           : public true_type {};
template<>           struct is_iteratable<SymbolicBlock>           : public true_type {};

template<typename T> struct is_stmt                           : public false_type {};
template<>           struct is_stmt<SymbolicStmt>           : public true_type {};


class MVisitor : public RecursiveASTVisitor<MVisitor> {
public:
	explicit MVisitor(CompilerInstance *CI)
    : astContext(&(CI->getASTContext())), block_(NULL), stmt_(NULL), expr_(NULL) // initialize private members
	{
        prog_ = new SymbolicProgram(this);
	}
    
    SymbolicExpr * EnterExpr(SymbolicExpr * expr) {
        SymbolicExpr * parent = expr_;
        expr_ = expr;
        return parent;
    }
    
    void LeaveExpr(SymbolicExpr * parent) {
        //stmt_->push_back(*expr_);
        expr_ = parent;
    }
    
    SymbolicStmt * EnterStmt(SymbolicStmt * stmt) {
        SymbolicStmt * parent = stmt_;
        stmt_ = stmt;
        return parent;
    }
    
    void LeaveStmt(SymbolicStmt * parent) {
        if (block_ == NULL) {
            prog_->push_back(*stmt_);
        } else {
            block_->push_back(*stmt_);
        }
        stmt_ = parent;
    }
    
    SymbolicBlock * EnterBlock(SymbolicBlock * block) {
        SymbolicBlock * parent = block_;
        block_ = block;
        return parent;
    }
    
    void LeaveBlock(SymbolicBlock * parent) {
        prog_->push_back(*block_);
        block_ = parent;
    }
    
    
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    
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
    
    SymbolicLiteral toSymbolicLiteral(APInt ii) {
        if (ii.getBitWidth() <= 64) {
            return SymbolicLiteral(ii.getSExtValue());
        } else {
            return SymbolicLiteral(ii.toString(10, ii.isSignBit()));
        }
    }
    
    SymbolicLiteral toSymbolicLiteral(const Expr * e) {
        clang::LangOptions LO;
        std::string str;
        raw_string_ostream ros(str);
        e->printPretty(ros, nullptr, astContext->getLangOpts());
        return SymbolicLiteral(str);
    }
    
    SymbolicExpr toSymbolicExpr(Qualifiers quals) {
        
        SymbolicQualifierExpr qualExp = SymbolicQualifierExpr(this);
        
        if (quals.hasConst()) {
            qualExp <<= "const";
        }
        if (quals.hasVolatile()) {
            qualExp <<= "volatile";
        }
        if (quals.hasRestrict()) {
            qualExp <<= "restrict";
        }
        if (quals.hasAddressSpace()) {
            unsigned addressSpace = quals.getAddressSpace();
            switch (addressSpace) {
                case LangAS::opencl_global:
                    qualExp <<= "__global";
                    break ;
                case LangAS::opencl_local:
                    qualExp <<= "__local";
                    break ;
                case LangAS::opencl_constant:
                    qualExp <<= "__constant";
                    break ;
                default:
                    qualExp <<= ToString("__attribute__((address_space(", addressSpace, ")))");
            }
        }
        return qualExp;
    }
    
    SymbolicExpr toSymbolicExpr(const Expr * e) {
        SymbolicExpr expr = SymbolicExpr(this);
        expr <<= toSymbolicLiteral(e);
        return expr;
    }
    
    SymbolicExpr toSymbolicExpr(const Type * ty) {
        if (const ConstantArrayType* arryT =
            dyn_cast<const ConstantArrayType>(ty)) {
            SymbolicLiteral len(toSymbolicLiteral(arryT->getSize()));
            SymbolicTypeExpr typ(this);
            typ <<= toSymbolicExpr(arryT->getElementType());
            SymbolicArrayExpr arry(this);
            arry <<= typ;
            arry <<= len;
            return arry;
        } else if (const VariableArrayType* arryT =
                   dyn_cast<const VariableArrayType>(ty)) {
            SymbolicExpr typ = toSymbolicExpr(arryT->getElementType());
            SymbolicExpr arry = SymbolicArrayExpr(this);
            arry <<= typ;
            if (arryT->getIndexTypeQualifiers().hasQualifiers()) {
                arry <<= toSymbolicExpr(arryT->getIndexTypeQualifiers());
            }
            if (arryT->getSizeModifier() == VariableArrayType::Static) {
                arry <<= "static";
            } else if (arryT->getSizeModifier() == VariableArrayType::Star) {
                arry <<= "*";
            }
            
            arry <<= SymbolicArraySizeExpr(this, toSymbolicLiteral(arryT->getSizeExpr()));
            return arry;
        }
        /*
        } else if (const IncompleteArrayType* arryT =
                   dyn_cast<const IncompleteArrayType>(ty)) {
            SymbolicExpr arry = CArray(this);
            SymbolicExpr typ = toSymbolicExpr(arryT->getElementType());
            arry << qualExp;
            arry << typ;
            return arry;
        } else if (const PointerType* pty = dyn_cast<const PointerType>(ty)) {
            const QualType pointee_ty = pty->getPointeeType();
            std::string name_ptr =  "* " + name;
            result = printDecl(pointee_ty, name_ptr, array_dim);
        } else if (const ElaboratedType* ety =
                   dyn_cast<const ElaboratedType>(ty)) {
            if (const RecordType* rty =
                dyn_cast<const RecordType>(ety->getNamedType().getTypePtr())) {
                TagDecl* tdecl = rty->getDecl();
                std::string def =
                rty->getDecl()->getName().str() + std::string(" ") + name;
                if (tdecl->isStruct()) result = "struct " + def;
                else if (tdecl->isUnion())  result = "union " + def;
                else {
                    assert(false &&
                           "PromoteDecl: only struct/union are allowed as record type.");
                }
            } else {
                assert(false && "PromoteDecl: unsupported elaborated type.");
            }
        } else if (const BuiltinType* bty = dyn_cast<const BuiltinType>(ty)) {
            clang::LangOptions LO;
            result = bty->getName(PrintingPolicy(LO)) + std::string(" ") + name;
        } else if (const TypedefType* tty = dyn_cast<const TypedefType>(ty)) {
            result = tty->getDecl()->getName().str() + std::string(" ") + name;
        } else if (const ParenType* pty = dyn_cast<const ParenType>(ty)) {
            QualType inner_ty = pty->getInnerType();
            std::string name_ptr =  "(" + name;
            result = printDecl(inner_ty, name_ptr, array_dim);
            result = result + ")";
        } else {
            llvm::errs() << "Error: " << ty->getTypeClassName() << '\n';
            assert(false && "PromoteDecl: unsupported type is found.");
        }
        */
        else if (const BuiltinType* bty = dyn_cast<const BuiltinType>(ty)) {
            SymbolicLiteral lit = bty->getName(PrintingPolicy(astContext->getLangOpts()));
            SymbolicTypeExpr exp(this);
            exp <<= lit;
            return exp;
        }
        return SymbolicNoneExpr(this);
    }
    
    SymbolicExpr toSymbolicExpr(QualType typ) {
        SymbolicExpr exp = SymbolicExpr(this);
        
        Qualifiers quals = typ.getLocalQualifiers();
        const Type * ty = typ.getTypePtr();
        
        exp <<= toSymbolicExpr(quals);
        exp <<= toSymbolicExpr(ty);
        
        return exp;
    }
    
    bool VisitVarDecl(VarDecl *D) {
		DEBUG;
		SymbolicAssignStmt stmt = SymbolicAssignStmt(this);
        stmt <<= toSymbolicExpr(D->getType());
        
        std::cout << stmt.toString() << std::endl;
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
    
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    
    bool VisitStmt(Stmt * stmt) {
        DEBUG;
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
    
    
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    
    bool VisitIntegerLiteral(IntegerLiteral *E) {
		DEBUG;
		E->dump();
        if (E->getType()->isUnsignedIntegerType()) {
            std::clog << "TODO;;;" << std::endl;
        } else if (E->getValue().getNumWords() == 1) {
            std::clog << " Signed " << E->getValue().getSExtValue() << std::endl;
        } else {
            std::clog << "TODO;;;" << std::endl;
        }
		return true;
    }
    
    bool VisitAbstractConditionalOperator(AbstractConditionalOperator * E) {
        DEBUG;
        E->dump();
        return true;
    }
    
    bool VisitAddrLabelExpr(AddrLabelExpr * E) {
        DEBUG;
        E->dump();
        return true;
    }
    
    bool VisitBinaryOperator(BinaryOperator * E) {
        DEBUG;
        E->dump();
        return true;
    }
    
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
    //VISIT(IntegerLiteral);
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
    
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    
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
SymbolicProgram * prog_;
};


SymbolicExpr::SymbolicExpr(MVisitor* owner) : len_(0), owner_(owner) {
    parent_ = owner_->EnterExpr(this);
}

SymbolicExpr::SymbolicExpr(MVisitor* owner, const SymbolicLiteral & lit) : len_(0), owner_(owner) {
    parent_ = owner_->EnterExpr(this);
    operator<<=(lit);
};

SymbolicExpr::SymbolicExpr(MVisitor* owner, const SymbolicExpr & exp) : len_(0), owner_(owner) {
    parent_ = owner_->EnterExpr(this);
    operator<<=(exp);
};

void SymbolicExpr::pop() {
    owner_->LeaveExpr(parent_);
}

SymbolicStmt::SymbolicStmt(MVisitor* owner, string head) : head_(head), owner_(owner) {
    parent_ = owner_->EnterStmt(this);
}

void SymbolicStmt::pop() {
    owner_->LeaveStmt(parent_);
}

SymbolicBlock::SymbolicBlock(MVisitor* owner) : owner_(owner) {
    parent_ = owner_->EnterBlock(this);
}

void SymbolicBlock::pop() {
    owner_->LeaveBlock(parent_);
}

SymbolicProgram::SymbolicProgram(MVisitor *owner) : owner_(owner) {
}

void SymbolicProgram::pop() {
    
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