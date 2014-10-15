

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
#include "ast/node/ast.hpp"
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
  explicit SVisitor(CompilerInstance &CI)
      : ctx(&(CI.getASTContext())), Traits(ctx->getCommentCommandTraits()),
        SM(ctx->getSourceManager()) {
    prog_ = shared_ptr<ProgramNode>(new ProgramNode());
    current_node = prog_;
  }

  shared_ptr<ProgramNode> getProgram() { return prog_; }

  shared_ptr<Node> toNode(const PresumedLoc &loc, const APInt &ii) {
    if (ii.getBitWidth() <= 64) {
      return shared_ptr<IntegerNode>(
          new IntegerNode(loc.getLine(), loc.getColumn(), ii.getSExtValue()));
    } else {
      return shared_ptr<StringNode>(new StringNode(
          loc.getLine(), loc.getColumn(), ii.toString(10, ii.isSignBit())));
    }
  }

  shared_ptr<StringNode> toNode(const PresumedLoc &loc, const Expr *e) {
    LangOptions LO;
    std::string str;
    raw_string_ostream ros(str);
    e->printPretty(ros, nullptr, ctx->getLangOpts());
    return shared_ptr<StringNode>(
        new StringNode(loc.getLine(), loc.getColumn(), str));
  }

  vector<shared_ptr<Node>> toNode(const PresumedLoc &loc,
                                  const Qualifiers &quals) {
    vector<shared_ptr<Node>> res;

    if (quals.hasConst()) {
      res.push_back(shared_ptr<IdentifierNode>(
          new IdentifierNode(loc.getLine(), loc.getColumn(), "const")));
    }
    if (quals.hasVolatile()) {
      res.push_back(shared_ptr<IdentifierNode>(
          new IdentifierNode(loc.getLine(), loc.getColumn(), "volatile")));
    }
    if (quals.hasRestrict()) {
      res.push_back(shared_ptr<IdentifierNode>(
          new IdentifierNode(loc.getLine(), loc.getColumn(), "restrict")));
    }
    if (quals.hasAddressSpace()) {
      unsigned addressSpace = quals.getAddressSpace();
      switch (addressSpace) {
      case LangAS::opencl_global:
        res.push_back(shared_ptr<IdentifierNode>(
            new IdentifierNode(loc.getLine(), loc.getColumn(), "__global")));
        break;
      case LangAS::opencl_local:
        res.push_back(shared_ptr<IdentifierNode>(
            new IdentifierNode(loc.getLine(), loc.getColumn(), "__local")));
        break;
      case LangAS::opencl_constant:
        res.push_back(shared_ptr<IdentifierNode>(
            new IdentifierNode(loc.getLine(), loc.getColumn(), "__constant")));
        break;
      default: {
        ostringstream o;
        o << "__attribute__((address_space(";
        o << addressSpace;
        o << ")))";
        res.push_back(shared_ptr<IdentifierNode>(
            new IdentifierNode(loc.getLine(), loc.getColumn(), o.str())));
      }
      }
    }
    return res;
  }
  shared_ptr<TypeNode> toNode(const PresumedLoc &loc, const Type *ty) {
    if (const BuiltinType *bty = dyn_cast<const BuiltinType>(ty)) {
      StringRef s = bty->getName(PrintingPolicy(ctx->getLangOpts()));
      return shared_ptr<TypeNode>(
          new TypeNode(loc.getLine(), loc.getColumn(), s));
    } else {
      return shared_ptr<TypeNode>(
          new TypeNode(loc.getLine(), loc.getColumn(), "unsupported"));
    }
  }

  shared_ptr<TypeNode> toNode(const PresumedLoc &loc, const QualType &typ) {
    shared_ptr<TypeNode> res;
    res = toNode(loc, typ.getTypePtr());
    if (typ.hasQualifiers()) {
      auto quals = toNode(loc, typ.getQualifiers());
      for (auto q : quals) {
        res->addQualifyer(q);
      }
    }
    return res;
  }
  shared_ptr<IdentifierNode> toNode(const PresumedLoc &loc, const Decl *decl) {
    const NamedDecl *ND = dyn_cast<NamedDecl>(decl);
    shared_ptr<IdentifierNode> nd(
        new IdentifierNode(loc.getLine(), loc.getColumn()));
    if (ND) {
      nd->setName(ND->getNameAsString());
      nd->isHidden(ND->isHidden());
    }
    if (const ValueDecl *VD = dyn_cast<ValueDecl>(decl)) {
      nd->setType(toNode(loc, VD->getType()));
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
         LangOptions LO;
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
            SymbolicLiteral lit = bty->getName(PrintingPolicy(ctx->getLangOpts()));
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

  bool canIgnoreCurrentASTNode() const {
    const DeclContext *decl = ctx->getTranslationUnitDecl();
    for (auto it = decl->decls_begin(), declEnd = decl->decls_end();
         it != declEnd; ++it) {
      auto startLocation = (*it)->getLocStart();
      if (startLocation.isValid() &&
          SM.getMainFileID() == SM.getFileID(startLocation)) {
        return false;
      }
    }
    return true;
  }
  virtual bool TraverseFunctionDecl(FunctionDecl *decl) {
    if (canIgnoreCurrentASTNode()) {
      return true;
    }
    PresumedLoc loc = SM.getPresumedLoc(decl->getPointOfInstantiation());
    shared_ptr<Node> tmp = current_node;
    shared_ptr<FunctionNode> func(
        new FunctionNode(loc.getLine(), loc.getColumn()));
    shared_ptr<TypeNode> returnType = toNode(loc, decl->getReturnType());
    shared_ptr<IdentifierNode> name(
        new IdentifierNode(loc.getLine(), loc.getColumn(),
                           decl->getNameInfo().getName().getAsString()));

    func->setReturnType(returnType);
    func->setName(name);
    current_node = tmp;
    unsigned paramCount = decl->getNumParams();
    for (unsigned i = 0; i < paramCount; i++) {
      TraverseDecl(decl->getParamDecl(i));
      func->addParameter(current_node);
    }
    if (decl->doesThisDeclarationHaveABody()) {
      shared_ptr<BlockNode> body = func->getBody();
      current_node = body;
      TraverseStmt(decl->getBody());
      *body <<= current_node;
    }
    current_node = func;
    return true;
  }

  virtual bool TraverseVarDecl(VarDecl *decl) {
    if (canIgnoreCurrentASTNode()) {
      return true;
    }
    PresumedLoc loc = SM.getPresumedLoc(decl->getSourceRange().getBegin());
    shared_ptr<Node> tmp = current_node;
    shared_ptr<DeclareNode> nd(new DeclareNode(loc.getLine(), loc.getColumn()));
    shared_ptr<TypeNode> typ = toNode(loc, decl->getType());
    shared_ptr<IdentifierNode> id(new IdentifierNode(
        loc.getLine(), loc.getColumn(), decl->getNameAsString()));

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
    if (canIgnoreCurrentASTNode()) {
      return true;
    }
    PresumedLoc loc = SM.getPresumedLoc(decl->getSourceRange().getBegin());
    shared_ptr<Node> tmp = current_node;
    shared_ptr<CompoundNode> nd(
        new CompoundNode(loc.getLine(), loc.getColumn()));
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
    if (canIgnoreCurrentASTNode()) {
      return true;
    }
    PresumedLoc loc = SM.getPresumedLoc(stmt->getWhileLoc());
    shared_ptr<WhileNode> nd(new WhileNode(loc.getLine(), loc.getColumn()));

    current_node = nd;
    TraverseStmt(stmt->getCond());

    current_node =
        shared_ptr<BlockNode>(new BlockNode(loc.getLine(), loc.getColumn()));
    TraverseStmt(stmt->getBody());
    *nd <<= current_node;

    current_node = nd;
    return true;
  }

  virtual bool TraverseForStmt(ForStmt *stmt) {
    PresumedLoc loc = SM.getPresumedLoc(stmt->getLocStart());
    shared_ptr<ForNode> nd(new ForNode(loc.getLine(), loc.getColumn()));
      if (stmt->getInit()) {
    current_node = nd;
      TraverseStmt(stmt->getInit());
      nd->setInit(current_node);
  }
      if (stmt->getCond()) {
    current_node = nd;
      TraverseStmt(stmt->getCond());
      nd->setCond(current_node);

  }
      if (stmt->getInc()) {
    current_node = nd;
      TraverseStmt(stmt->getInc());
      nd->setInc(current_node);
  }
      if (stmt->getBody()) {
    current_node = nd;
      TraverseStmt(stmt->getBody());
      nd->setBody(current_node);
  }
    current_node = nd;
    return true;
  }
  virtual bool TraverseIfStmt(IfStmt *stmt) {
    shared_ptr<Node> tmp = current_node;
    PresumedLoc loc = SM.getPresumedLoc(stmt->getIfLoc());
    shared_ptr<IfNode> nd(new IfNode(loc.getLine(), loc.getColumn()));

    current_node = nd;
    TraverseStmt(stmt->getCond());
    nd->setCondition(current_node);

    current_node = nd;
    TraverseStmt(stmt->getThen());
    if (!current_node->isBlock()) {
      shared_ptr<BlockNode> blk(new BlockNode(loc.getLine(), loc.getColumn()));
      *blk <<= current_node;
      current_node = blk;
    }
    nd->setThen(current_node);

    if (stmt->getElse() != NULL) {
      current_node = nd;
      TraverseStmt(stmt->getElse());
      if (!current_node->isBlock()) {
        shared_ptr<BlockNode> blk(
            new BlockNode(loc.getLine(), loc.getColumn()));
        *blk <<= current_node;
        current_node = blk;
      }
      nd->setElse(current_node);
    }
    current_node = nd;

    return true;
  }

  virtual bool TraverseCompoundStmt(CompoundStmt *stmt) {
    shared_ptr<Node> tmp = current_node;
    PresumedLoc loc = SM.getPresumedLoc(stmt->getLocStart());
    shared_ptr<CompoundNode> nd(
        new CompoundNode(loc.getLine(), loc.getColumn()));

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
    PresumedLoc loc = SM.getPresumedLoc(stmt->getReturnLoc());
    shared_ptr<ReturnNode> nd(new ReturnNode(loc.getLine(), loc.getColumn()));

    current_node = nd;
    if (stmt->getRetValue()) {
      TraverseStmt(stmt->getRetValue());

      nd->setReturnValue(current_node);
    }

    current_node = nd;
    return true;
  }
  virtual bool handleUnaryOperator(UnaryOperator *E) {
    PresumedLoc loc = SM.getPresumedLoc(E->getExprLoc());
    shared_ptr<UnaryOperatorNode> nd(
        new UnaryOperatorNode(loc.getLine(), loc.getColumn()));
    current_node = nd;
    nd->setOperator(E->getOpcodeStr(E->getOpcode()));
    TraverseStmt(E->getSubExpr());
    nd->setArg(current_node);
    current_node = nd;
  }

#define OPERATOR(NAME)                                                         \
  bool TraverseUnary##NAME(UnaryOperator *E) {                                 \
    return handleUnaryOperator(E);                                                               \
  }

  UNARYOP_LIST()
#undef OPERATOR
  virtual bool TraverseUnaryOperator(UnaryOperator * op) {
      return handleUnaryOperator(op);
  }
  virtual bool TraverseUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr * E) {
      DEBUG;
      E->dumpColor();
      current_node = shared_ptr<StringNode>(new StringNode(-1, -1, string("TODOXXUnaryExprOrTypeTraitExpr")));
      return true;
  }

  virtual bool handleBinaryOperator(BinaryOperator *E) {
    PresumedLoc loc = SM.getPresumedLoc(E->getOperatorLoc());
    if (E->isAssignmentOp()) {
      shared_ptr<AssignNode> nd(new AssignNode(loc.getLine(), loc.getColumn()));
      current_node = nd;
      TraverseStmt(E->getLHS());
      nd->setLHS(current_node);
      current_node = nd;
      TraverseStmt(E->getRHS());
      nd->setRHS(current_node);
      current_node = nd;
    } else {
      shared_ptr<BinaryOperatorNode> nd(
          new BinaryOperatorNode(loc.getLine(), loc.getColumn()));
      current_node = nd;
      nd->setOperator(E->getOpcodeStr());
      TraverseStmt(E->getLHS());
      nd->setLHS(current_node);
      current_node = nd;
      TraverseStmt(E->getRHS());
      if (current_node == nd) {
          std::cout << "Something has went wrong " << E->getOpcodeStr().str() << std::endl;
          E->dumpColor();
      }
      nd->setRHS(current_node);
      current_node = nd;
    }
    return true;
  }
  virtual bool TraverseBinaryOperator(BinaryOperator * op) {
      return handleBinaryOperator(op);
  }

#define GENERAL_BINOP_FALLBACK(NAME, BINOP_TYPE)                               \
  bool TraverseBin##NAME(BINOP_TYPE *E) {                                      \
    return handleBinaryOperator(E);                                                               \
  }
#define OPERATOR(NAME) GENERAL_BINOP_FALLBACK(NAME, BinaryOperator)
  BINOP_LIST()
#undef OPERATOR

#define OPERATOR(NAME)                                                         \
  GENERAL_BINOP_FALLBACK(NAME##Assign, CompoundAssignOperator)
  CAO_LIST()
#undef GENERAL_BINOP_FALLBACK
#undef OPERATOR

  virtual bool TraverseDeclRefExpr(DeclRefExpr *E) {
    const ValueDecl *D = E->getDecl();
    PresumedLoc loc = SM.getPresumedLoc(E->getLocation());

    current_node = shared_ptr<IdentifierNode>(
        new IdentifierNode(loc.getLine(), loc.getColumn(), "unkownid"));
    if (D) {
      current_node = toNode(loc, D);
    }

    const NamedDecl *FD = E->getFoundDecl();
    if (FD && D != FD) {
      current_node = toNode(loc, FD);
    }
    return true;
  }
  virtual bool TraverseCallExpr(CallExpr *E) {
    const FunctionDecl *F = E->getDirectCallee();
    PresumedLoc loc = SM.getPresumedLoc(F->getLocation());
    DEBUG;
    shared_ptr<CallNode> call(new CallNode(loc.getLine(), loc.getColumn()));
    current_node = call;
    call->setFunction(shared_ptr<IdentifierNode>(
        new IdentifierNode(loc.getLine(), loc.getColumn(),
                           F->getNameInfo().getName().getAsString())));
    for (auto arg : E->arguments()) {
      TraverseStmt(arg);
      call->addArg(current_node);
      current_node = call;
    }
    return true;
  }
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/
  /*******************************************************************************************************/

  virtual bool TraverseIntegerLiteral(IntegerLiteral *E) {
    PresumedLoc loc = SM.getPresumedLoc(E->getExprLoc());
    current_node = toNode(loc, E->getValue());
    return true;
  }
  virtual bool TraverseCharacterLiteral(CharacterLiteral *E) {
    PresumedLoc loc = SM.getPresumedLoc(E->getExprLoc());
    current_node = shared_ptr<CharacterNode>(
        new CharacterNode(loc.getLine(), loc.getColumn(), E->getValue()));
    return true;
  }

  virtual bool TraverseStringLiteral(StringLiteral *E) {
    // cout << "striiiing" << endl;
    PresumedLoc loc = SM.getPresumedLoc(E->getExprLoc());
    current_node = shared_ptr<StringNode>(
        new StringNode(loc.getLine(), loc.getColumn(), E->getString().str()));
    return true;
  }

  void addCurrent() {
    *prog_ <<= current_node;
    current_node = prog_;
  }

  bool shouldVisitTemplateInstantiations() const { return false; }
  bool shouldVisitImplicitCode() const { return false; }

private:
  ASTContext *ctx; // used for getting additional AST info
  shared_ptr<ProgramNode> prog_;
  shared_ptr<Node> current_node;

  const CommandTraits &Traits;
  const SourceManager &SM;
};

class PreprocessorCallback : public PPCallbacks {
  Preprocessor &PP;
  bool disabled = false; // To prevent recurstion

public:
  PreprocessorCallback(Preprocessor &PP) : PP(PP) {}
  ~PreprocessorCallback() {}

  void MacroExpands(const Token &MacroNameTok, const MacroInfo *MI,
                    SourceRange Range) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDirective *MD,
                    SourceRange Range, const MacroArgs *Args) override {
    MacroExpands(MacroNameTok, MD->getMacroInfo(), Range);
  }

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          llvm::StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange, const FileEntry *File,
                          llvm::StringRef SearchPath,
                          llvm::StringRef RelativePath,
                          const Module *Imported) override {

    std::cout << "This is an include" << std::endl;
  }
  virtual void If(SourceLocation Loc, SourceRange ConditionRange,
                  ConditionValueKind ConditionValue) override {
    HandlePPCond(Loc, Loc);
  }
  virtual void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
                      const MacroDirective *MD) override {
    HandlePPCond(Loc, Loc);
  }
  virtual void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
                     const MacroDirective *MD) override {
    HandlePPCond(Loc, Loc);
  }
  virtual void Elif(SourceLocation Loc, SourceRange ConditionRange,
                    ConditionValueKind ConditionValue,
                    SourceLocation IfLoc) override {
    ElifMapping[Loc] = IfLoc;
    HandlePPCond(Loc, IfLoc);
  }
  virtual void Else(SourceLocation Loc, SourceLocation IfLoc) override {
    HandlePPCond(Loc, IfLoc);
  }
  virtual void Endif(SourceLocation Loc, SourceLocation IfLoc) override {
    HandlePPCond(Loc, IfLoc);
  }
  virtual bool FileNotFound(llvm::StringRef FileName,
                            llvm::SmallVectorImpl<char> &RecoveryPath) {
    if (!PP.GetSuppressIncludeNotFoundError()) {
      PP.SetSuppressIncludeNotFoundError(true);
    }
    return false;
  }

private:
  std::map<SourceLocation, SourceLocation>
      ElifMapping; // Map an elif location to the real if;
  void HandlePPCond(SourceLocation Loc, SourceLocation IfLoc) {}
};

struct SDiagnosticConsumer : DiagnosticConsumer {
  SDiagnosticConsumer() {}
  int HadRealError = 0;
  virtual void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel,
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
      : CI(CI), Visitor(new SVisitor(CI)),
      Ctx(CI.getASTContext()),
      SM(CI.getASTContext().getSourceManager()) {
    // ci.getPreprocessor().enableIncrementalProcessing();
  }
  virtual void Initialize(ASTContext &Ctx) override {
      mainFileID = SM.getMainFileID();
  }


  // override this to call our SVisitor on each top-level Decl
  virtual void HandleTranslationUnit(ASTContext &context) {
    DEBUG;
    // visitor->TraverseDecl(context.getTranslationUnitDecl());
    // find all C++ #include needed for the converted C++ types
    auto collectInclude =
        [&](clang::ASTContext &i_ctx, const clang::QualType &i_type) {
      auto decl = i_type->getAsCXXRecordDecl();
      if (decl != nullptr) {
        decl->dump();
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
  virtual bool HandleTopLevelDecl(DeclGroupRef dg) {
    if (CI.getDiagnostics().hasFatalErrorOccurred()) {
      // Reset errors: (Hack to ignore the fatal errors.)
      CI.getDiagnostics().Reset();
      // When there was fatal error, processing the warnings may cause crashes
    }
    for (auto decl : dg) {
      //(*iter).dump();
      if (SM.getFileID(SM.getExpansionLoc(decl->getLocation()))!=mainFileID)
        continue;
      DEBUG;
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
                                                 StringRef file) {

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

    // CI.getPreprocessor().addPPCallbacks(unique_ptr<PreprocessorCallback>(
    //    new PreprocessorCallback(CI.getPreprocessor())));
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
    // eventually I would have to create a new CI and this would not be an issue3
   // CI.createDiagnostics();
     // CI.getDiagnosticOpts().ShowColors = 1;

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
  printer.print(st, stdout);

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

  ostringstream o;
  o << "#include <cstdio>" << std::endl;
  o << "void f(int x, int y, int z) {" << std::endl;
  o << "return ;" << std::endl;
  o << "}" << std::endl;
  o << "int main() {" << std::endl;
  o << "const char v = 'g', s = 2; int g; return g + v;" << std::endl;
  o << "if (v == g) { return ; }" << std::endl;
  o << "for (int dev = 0; dev < 100; dev++) {" << std::endl;
  o << "f(1,2,3);" << std::endl;
  o << "printf(\"%d %d %d \", 1,2,3);" << std::endl;
  o << "}" << std::endl;
  o << "}" << std::endl;

  runToolOnCodeWithArgs(newFrontendActionFactory<SFrontendAction>()->create(),
                        o.str(), args);
  // print out the rewritten source code ("rewriter" is a global var.)
  // rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

  // llvm::DeleteContainerPointers(ASTs);
}
