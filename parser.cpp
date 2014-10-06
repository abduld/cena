

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

#include <string>
#include <vector>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::comments;
using namespace clang::driver;
using namespace clang::tooling;

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

class SVisitor : public RecursiveASTVisitor<SVisitor> {
public:
  explicit SVisitor(CompilerInstance *CI) : astContext(&(CI->getASTContext())), SM(CI->getASTContext().getSourceManager()), Traits(CI->getASTContext().getCommentCommandTraits()) {
    prog_ = shared_ptr<ProgramNode>(new ProgramNode());
    current_node = prog_;
  }

  shared_ptr<ProgramNode> getProgram() {
    return prog_;
  }


    shared_ptr<Node> toNode(APInt ii) {
        if (ii.getBitWidth() <= 64) {
            return shared_ptr<IntegerNode>(new IntegerNode(ii.getSExtValue()));
        } else {
            return shared_ptr<StringNode>(new StringNode(ii.toString(10, ii.isSignBit())));
        }
    }
    shared_ptr<TypeNode> toNode(QualType typ) {
        return shared_ptr<TypeNode>(new TypeNode());
    }
    #if 0
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
    
    shared_ptr<Node> toNode(QualType typ) {
        shared_ptr<Type> typ(new Type());
        
        typ <<= toNode(typ.getLocalQualifiers());
        typ <<= toNode(typ.getTypePtr());
        
        return typ;
    }
    #endif
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/




    virtual bool TraverseVarDecl(VarDecl *decl){
        shared_ptr<DeclareNode> nd(new DeclareNode());
        shared_ptr<TypeNode> typ = toNode(decl->getType());
        shared_ptr<IdentifierNode> id(new IdentifierNode(decl->getNameAsString()));
        PresumedLoc PLoc = SM.getPresumedLoc(decl->getSourceRange().getBegin());
        
        nd->setType(typ);
        nd->setIdentifier(id);

        if (decl->hasInit()) {
          shared_ptr<Node> tmp = current_node;
          current_node = nd;
          TraverseStmt(decl->getInit());
          nd->setInitializer(current_node);
          current_node = tmp;
        }
        *current_node <<= nd;
        return true;
    }  

    virtual bool TraverseDeclStmt(DeclStmt *decl){
      shared_ptr<Node> tmp = current_node;
      shared_ptr<CompoundNode> nd(new CompoundNode());
        for(auto init = decl->decl_begin(), end = decl->decl_end(); init!=end ; ++init) {
            current_node = tmp;
            TraverseDecl(*init);
            *nd <<= current_node;
        }
        current_node = tmp;
        *current_node <<= nd;
        return true;
    }


    virtual bool TraverseWhileStmt(WhileStmt *stmt) {
      shared_ptr<Node> tmp = current_node;
        shared_ptr<WhileNode> nd(new WhileNode());

        PresumedLoc PLoc = SM.getPresumedLoc(whSt->getWhileLoc());

        current_node = nd;        
        TraverseStmt(stmt->getCond());

        current_node = shared_ptr<BlockNode>(new BlockNode());
        TraverseStmt(stmt->getBody());
        *nd <<= current_node;

        current_node = tmp;
        *current_node <<= nd;
        return true;
    }   
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/

  bool VisitIntegerLiteral(IntegerLiteral *E) {
    if (E->getType()->isUnsignedIntegerType()) {
            std::clog << "TODO;;;" << std::endl;
        } else if (E->getValue().getNumWords() == 1) {
    *current_node <<= toNode(E->getValue());
        } else {
            std::clog << "TODO;;;" << std::endl;
        }
    return true;
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
  virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
    // a DeclGroupRef may have multiple Decls, so we iterate through each one
    for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
      Decl *D = *i;
      visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
    }
    return true;
  }
};

class SFrontendAction : public ASTFrontendAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                         StringRef file) {
    std::unique_ptr<ASTConsumer> astcons( new SASTConsumer(&CI));
    //prog_ = astcons->getProgram();
    return std::move(astcons); // pass CI pointer to ASTConsumer
  }
  shared_ptr<ProgramNode> getProgram() {
    return prog_;
  }
private:
  shared_ptr<ProgramNode> prog_;
};

void parse() {
  llvm::sys::PrintStackTraceOnErrorSignal();

  IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOpts(
      new clang::DiagnosticOptions());
  std::unique_ptr<clang::DiagnosticsEngine> diagnostics(
      new clang::DiagnosticsEngine(
          llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(
              new clang::DiagnosticIDs()),
          &*diagnosticOpts, new clang::IgnoringDiagConsumer(), true));

  std::unique_ptr<CompilationDatabase> Compilations(
      new FixedCompilationDatabase("/", vector<string>()));

  std::vector<string> args;
  args.push_back("-std=c++11");

  runToolOnCodeWithArgs(
      newFrontendActionFactory<SFrontendAction>()->create(),
      "int g = 3, y =4; int main() { return 1; }", args);
  // print out the rewritten source code ("rewriter" is a global var.)
  // rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

  // llvm::DeleteContainerPointers(ASTs);
}
