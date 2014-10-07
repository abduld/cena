

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
#include "symbolic/ast.hpp"
#include "external/backward_cpp/backward.hpp"

#include <string>
#include <vector>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::comments;
using namespace clang::driver;
using namespace clang::tooling;
using namespace backward;

#define DEBUG printf("DEBUG :: >>> %s %d ... \n", __PRETTY_FUNCTION__, __LINE__)

// All unary operators.
#define UNARYOP_LIST()                                                         \
  \
OPERATOR(PostInc) OPERATOR(PostDec) OPERATOR(PreInc) OPERATOR(PreDec) \
OPERATOR(AddrOf) OPERATOR(Deref) OPERATOR(Plus) OPERATOR(Minus) \
OPERATOR(Not) OPERATOR(LNot) OPERATOR(Real) OPERATOR(Imag) \
OPERATOR(Extension)

// All binary operators (excluding compound assign operators).
#define BINOP_LIST()                                                           \
  \
OPERATOR(PtrMemD) OPERATOR(PtrMemI) OPERATOR(Mul) OPERATOR(Div) \
OPERATOR(Rem) OPERATOR(Add) OPERATOR(Sub) OPERATOR(Shl) OPERATOR(Shr) \
OPERATOR(LT) OPERATOR(GT) OPERATOR(LE) OPERATOR(GE) OPERATOR(EQ) \
OPERATOR(NE) OPERATOR(And) OPERATOR(Xor) OPERATOR(Or) OPERATOR(LAnd) \
OPERATOR(LOr) OPERATOR(Assign) OPERATOR(Comma)

// All compound assign operators.
#define CAO_LIST()                                                             \
  \
OPERATOR(Mul) OPERATOR(Div) OPERATOR(Rem) OPERATOR(Add) OPERATOR(Sub) \
OPERATOR(Shl) OPERATOR(Shr) OPERATOR(And) OPERATOR(Or) OPERATOR(Xor)

StackTrace st;
void collect_trace() { st.load_here(); }
class SVisitor : public RecursiveASTVisitor<SVisitor> {
public:
  explicit SVisitor(CompilerInstance *CI)
      : astContext(&(CI->getASTContext())),
        Traits(astContext->getCommentCommandTraits()),
        SM(astContext->getSourceManager()) {
    prog_ = shared_ptr<ProgramNode>(new ProgramNode());
    current_node = prog_;
  }

  shared_ptr<ProgramNode> getProgram() { return prog_; }

  shared_ptr<Node> toNode(APInt ii) {
    if (ii.getBitWidth() <= 64) {
      return shared_ptr<IntegerNode>(new IntegerNode(ii.getSExtValue()));
    } else {
      return shared_ptr<StringNode>(
          new StringNode(ii.toString(10, ii.isSignBit())));
    }
  }

  shared_ptr<StringNode> toNode(const Expr *e) {
    clang::LangOptions LO;
    std::string str;
    raw_string_ostream ros(str);
    e->printPretty(ros, nullptr, astContext->getLangOpts());
    return shared_ptr<StringNode>(new StringNode(str));
  }


    vector<shared_ptr<Node>> toNode(const Qualifiers & quals) {
        vector<shared_ptr<Node>> res;
        
        if (quals.hasConst()) {
            res.push_back(shared_ptr<StringNode>(new StringNode("const")));
        }
        if (quals.hasVolatile()) {
            res.push_back(shared_ptr<StringNode>(new StringNode("volatile")));
        }
        if (quals.hasRestrict()) {
            res.push_back(shared_ptr<StringNode>(new StringNode("restrict")));
        }
        if (quals.hasAddressSpace()) {
            unsigned addressSpace = quals.getAddressSpace();
            switch (addressSpace) {
                case LangAS::opencl_global:
            res.push_back(shared_ptr<StringNode>(new StringNode("__global")));
                    break ;
                case LangAS::opencl_local:
            res.push_back(shared_ptr<StringNode>(new StringNode("__local")));
                    break ;
                case LangAS::opencl_constant:
            res.push_back(shared_ptr<StringNode>(new StringNode("__constant")));
                    break ;
                default:
                {
                  ostringstream o;
                  o<<"__attribute__((address_space(";
                    o<<addressSpace;
                    o<< ")))";
            res.push_back(shared_ptr<StringNode>(new StringNode(o.str())));
                }
            }
        }
        return res;
    }
  shared_ptr<TypeNode> toNode(const Type *ty) {
    if (const BuiltinType *bty = dyn_cast<const BuiltinType>(ty)) {
      StringRef s = bty->getName(PrintingPolicy(astContext->getLangOpts()));
      return shared_ptr<TypeNode>(new TypeNode(s));
    } else {
      return shared_ptr<TypeNode>(new TypeNode("unsupported"));
    }
  }

  shared_ptr<TypeNode> toNode(const QualType & typ) {
    shared_ptr<TypeNode> res;
    res = toNode(typ.getTypePtr());
    if (typ.hasQualifiers()) {
        auto quals = toNode(typ.getQualifiers());
        for (auto q: quals) {
          res->addQualifyer(q);
        }
      }
    return res;
  }
  shared_ptr<IdentifierNode> toNode(const Decl * decl) {
    const NamedDecl *ND = dyn_cast<NamedDecl>(decl);
    shared_ptr<IdentifierNode> nd(new IdentifierNode());
    if (ND) {
      nd->setName(ND->getNameAsString());
      nd->isHidden(ND->isHidden());
    }
    if (const ValueDecl *VD = dyn_cast<ValueDecl>(decl)) {
      nd->setType(toNode(VD->getType()));
    }
    return nd;
  }

#if 0
    
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

#endif
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  virtual bool TraverseFunctionDecl(FunctionDecl *decl) {
    shared_ptr<Node> tmp = current_node;
    shared_ptr<FunctionNode> func(new FunctionNode());
    shared_ptr<TypeNode> returnType = toNode(decl->getReturnType());
    shared_ptr<IdentifierNode> name(
        new IdentifierNode(decl->getNameInfo().getName().getAsString()));

    func->setReturnType(returnType);
    func->setName(name);
    current_node = tmp;
    unsigned paramCount = decl->getNumParams();
    for (unsigned i = 0; i < paramCount; i++) {
      TraverseDecl(decl->getParamDecl(i));
      func->addParameter(current_node);
    }
    shared_ptr<BlockNode> body = func->getBody();
    current_node = body;
    TraverseStmt(decl->getBody());
    *body <<= current_node;
    current_node = func;
    return true;
  }
  virtual bool TraverseVarDecl(VarDecl *decl) {
    shared_ptr<Node> tmp = current_node;
    shared_ptr<DeclareNode> nd(new DeclareNode());
    shared_ptr<TypeNode> typ = toNode(decl->getType());
    shared_ptr<IdentifierNode> id(new IdentifierNode(decl->getNameAsString()));
    PresumedLoc PLoc = SM.getPresumedLoc(decl->getSourceRange().getBegin());

    nd->setType(typ);
    nd->setIdentifier(id);

    if (decl->hasInit()) {
      current_node = nd;
      TraverseStmt(decl->getInit());
      assert(current_node != nd);
      nd->setInitializer(current_node);
    }
    current_node = nd;
    return true;
  }

  virtual bool TraverseDeclStmt(DeclStmt *decl) {
    shared_ptr<Node> tmp = current_node;
    shared_ptr<CompoundNode> nd(new CompoundNode());
    for (auto init = decl->decl_begin(), end = decl->decl_end(); init != end;
         ++init) {
      current_node = tmp;
      TraverseDecl(*init);
      *nd <<= current_node;
    }
    current_node = nd;
    return true;
  }

  virtual bool TraverseWhileStmt(WhileStmt *stmt) {
    shared_ptr<WhileNode> nd(new WhileNode());

    PresumedLoc PLoc = SM.getPresumedLoc(stmt->getWhileLoc());

    current_node = nd;
    TraverseStmt(stmt->getCond());

    current_node = shared_ptr<BlockNode>(new BlockNode());
    TraverseStmt(stmt->getBody());
    *nd <<= current_node;

    current_node = nd;
    return true;
  }

  virtual bool TraverseCompoundStmt(CompoundStmt *stmt) {
    shared_ptr<Node> tmp = current_node;
    shared_ptr<CompoundNode> nd(new CompoundNode());

    for (auto init = stmt->body_begin(), end = stmt->body_end(); init != end;
         ++init) {
      current_node = nd;
      TraverseStmt(*init);
      *nd <<= current_node;
    }
    current_node = nd;
    return true;
  }
  virtual bool TraverseReturnStmt(ReturnStmt *stmt) {
    shared_ptr<ReturnNode> nd(new ReturnNode());

    PresumedLoc PLoc = SM.getPresumedLoc(stmt->getReturnLoc());

    current_node = nd;
    if (stmt->getRetValue()) {
      TraverseStmt(stmt->getRetValue());

    std::cout << current_node->toString() << std::endl;
      nd->setReturnValue(current_node);
    }

    current_node = nd;
    return true;
  }

#define OPERATOR(NAME)                                           \
  bool TraverseUnary##NAME(UnaryOperator *E) {                  \
      shared_ptr<UnaryOperatorNode> nd(new UnaryOperatorNode());\
      current_node = nd;\
      nd->setOperator(string(#NAME));\
      TraverseStmt(E->getSubExpr());\
      nd->setArg(current_node);\
      current_node = nd;\
    return true;                                                \
  }     

  UNARYOP_LIST()
#undef OPERATOR

#define GENERAL_BINOP_FALLBACK(NAME, BINOP_TYPE)                \
  bool TraverseBin##NAME(BINOP_TYPE *E) {                       \
      shared_ptr<BinaryOperatorNode> nd(new BinaryOperatorNode());\
      current_node = nd;\
      nd->setOperator(E->getOpcodeStr());\
      TraverseStmt(E->getLHS());\
      nd->setLHS(current_node);\
      current_node = nd;\
      TraverseStmt(E->getRHS());\
      nd->setRHS(current_node);\
      current_node = nd;\
    return true;                                                \
  }        
#define OPERATOR(NAME) GENERAL_BINOP_FALLBACK(NAME, BinaryOperator)
  BINOP_LIST()
#undef OPERATOR

#define OPERATOR(NAME) \
  GENERAL_BINOP_FALLBACK(NAME##Assign, CompoundAssignOperator)
  CAO_LIST()
#undef GENERAL_BINOP_FALLBACK
#undef OPERATOR


  virtual bool TraverseBinaryOperator(BinaryOperator *E) {
    PresumedLoc PLoc = SM.getPresumedLoc(E->getOperatorLoc());
    if (E->isAssignmentOp()) {
      shared_ptr<AssignNode> nd(new AssignNode());
      current_node = nd;
      TraverseStmt(E->getLHS());
      nd->setLHS(current_node);
      current_node = nd;
      TraverseStmt(E->getRHS());
      nd->setRHS(current_node);
      current_node = nd;
    } else {
      shared_ptr<BinaryOperatorNode> nd(new BinaryOperatorNode());
      current_node = nd;
      nd->setOperator(E->getOpcodeStr());
      TraverseStmt(E->getLHS());
      nd->setLHS(current_node);
      current_node = nd;
      TraverseStmt(E->getRHS());
      nd->setRHS(current_node);
      current_node = nd;
    }
DEBUG;
    return true;
  }
    virtual bool TraverseDeclRefExpr(DeclRefExpr * E) {
DEBUG;
      const ValueDecl *D = E->getDecl();
        current_node = shared_ptr<IdentifierNode>(new IdentifierNode("unkownid"));
      if (D) {
        current_node = toNode(D);
      }

  const NamedDecl *FD = E->getFoundDecl();
  if (FD && D != FD) {
        current_node = toNode(FD);
  }
        return true;
    }
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/

  virtual bool TraverseIntegerLiteral(IntegerLiteral *E) {
    if (E->getType()->isUnsignedIntegerType()) {
      std::clog << "TODO;;;" << std::endl;
    } else if (E->getValue().getNumWords() == 1) {
      current_node = toNode(E->getValue());
    } else {
      std::clog << "TODO;;;" << std::endl;
    }
    return true;
  }
  virtual bool TraverseCharacterLiteral(CharacterLiteral *E) {
    current_node = shared_ptr<CharacterNode>(new CharacterNode(E->getValue()));
    return true;
  }

    virtual bool TraverseStringLiteral(StringLiteral *E) {
        //cout << "striiiing" << endl;
      current_node = shared_ptr<StringNode>(new StringNode(E->getString().str()));
        return true;
    }

  void addCurrent() {
    *prog_ <<= current_node;
    current_node = prog_;
  }

private:
  clang::ASTContext *astContext; // used for getting additional AST info
  shared_ptr<ProgramNode> prog_;
  shared_ptr<Node> current_node;

  const CommandTraits &Traits;
  const SourceManager &SM;
};

class SASTConsumer : public ASTConsumer {
private:
  SVisitor *visitor;

public:
  explicit SASTConsumer(CompilerInstance *CI) : visitor(new SVisitor(CI)) {}

  /*
      virtual void HandleTranslationUnit(ASTContext &Ctx) {
              visitor->TraverseDecl(Ctx.getTranslationUnitDecl());
      }
   */

  // override this to call our SVisitor on each top-level Decl
  virtual void HandleTranslationUnit(ASTContext &context) {
    visitor->TraverseDecl(context.getTranslationUnitDecl());
    std::cout << "Program : " << std::endl;
    std::cout << getProgram()->toCCode() << std::endl;
    return;
  }
  virtual bool HandleTopLevelDecl(DeclGroupRef dg) {
    for (auto iter : dg) {
      visitor->TraverseDecl(iter);
      visitor->addCurrent();
    }
    return true;
  }
  shared_ptr<ProgramNode> getProgram() { return visitor->getProgram(); }
};

class SFrontendAction : public ASTFrontendAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) {
    astcons = std::unique_ptr<SASTConsumer>(new SASTConsumer(&CI));
    return std::move(astcons); // pass CI pointer to ASTConsumer
  }
  shared_ptr<ProgramNode> getProgram() {
    prog_ = astcons->getProgram();

    return prog_;
  }

private:
  std::unique_ptr<SASTConsumer> astcons;
  shared_ptr<ProgramNode> prog_;
};
void parse() {
  llvm::sys::PrintStackTraceOnErrorSignal();
  collect_trace();
  Printer printer;
  printer.print(st, stdout);
  IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOpts(
      new clang::DiagnosticOptions());
  std::unique_ptr<clang::DiagnosticsEngine> diagnostics(
      new clang::DiagnosticsEngine(
          llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(
              new clang::DiagnosticIDs()),
          &*diagnosticOpts, new clang::IgnoringDiagConsumer(), true));
  diagnostics->setSuppressSystemWarnings(true); 
  diagnostics->setIgnoreAllWarnings(true);
  std::unique_ptr<CompilationDatabase> Compilations(
      new FixedCompilationDatabase("/", vector<string>()));

  std::vector<string> args;
  args.push_back("-std=c++11 -O0");

  runToolOnCodeWithArgs(newFrontendActionFactory<SFrontendAction>()->create(),
                        "int main() { const char v = 'g', s = 2; int g; return g + v;}",
                        args);
  // print out the rewritten source code ("rewriter" is a global var.)
  // rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

  // llvm::DeleteContainerPointers(ASTs);
}
