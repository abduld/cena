
#ifndef __VISITOR_H__
#define __VISITOR_H__

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
  explicit SVisitor(CompilerInstance &CI);

  shared_ptr<ProgramNode> getProgram();

  bool TraverseFunctionDecl(FunctionDecl *decl);
  bool TraverseVarDecl(VarDecl *decl);
  bool TraverseDeclStmt(DeclStmt *decl);
  bool TraverseWhileStmt(WhileStmt *stmt);
  bool TraverseForStmt(ForStmt *stmt);
  bool TraverseIfStmt(IfStmt *stmt);
  bool TraverseCompoundStmt(CompoundStmt *stmt);
#define OPERATOR(NAME)                                                         \
  bool TraverseUnary##NAME(UnaryOperator *E);
  UNARYOP_LIST()
#undef OPERATOR
  bool TraverseUnaryOperator(UnaryOperator *op);
  bool TraverseUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *E);
  bool TraverseBinaryOperator(BinaryOperator *op);
#define GENERAL_BINOP_FALLBACK(NAME, BINOP_TYPE)                               \
  bool TraverseBin##NAME(BINOP_TYPE *E);
#define OPERATOR(NAME) GENERAL_BINOP_FALLBACK(NAME, BinaryOperator)
  BINOP_LIST()
#undef OPERATOR

#define OPERATOR(NAME)                                                         \
  GENERAL_BINOP_FALLBACK(NAME##Assign, CompoundAssignOperator)
  CAO_LIST()
#undef GENERAL_BINOP_FALLBACK
#undef OPERATOR
  bool TraverseReturnStmt(ReturnStmt *stmt);
  bool TraverseDeclRefExpr(DeclRefExpr *E);
  bool TraverseCallExpr(CallExpr *E);
  bool TraverseIntegerLiteral(IntegerLiteral *E);
  bool TraverseCharacterLiteral(CharacterLiteral *E);
  bool TraverseStringLiteral(StringLiteral *E);
  bool shouldVisitTemplateInstantiations() const { return true; }
  bool shouldVisitImplicitCode() const { return true; }

  void addCurrent();
private:
  ASTContext *ctx;
  shared_ptr<ProgramNode> prog_;
  shared_ptr<Node> current_node;

  const comments::CommandTraits &Traits;
  const SourceManager &SM;
  bool handleUnaryOperator(UnaryOperator *E);
  bool handleBinaryOperator(BinaryOperator *E);
  bool canIgnoreCurrentASTNode() const;
};

#endif /* __VISITOR_H__ */



