
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

  virtual bool TraverseFunctionDecl(FunctionDecl *decl) override;
  virtual bool TraverseVarDecl(VarDecl *decl) override;
  virtual bool TraverseDeclStmt(DeclStmt *decl) override;
  virtual bool TraverseWhileStmt(WhileStmt *stmt) override;
  virtual bool TraverseForStmt(ForStmt *stmt) override;
  virtual bool TraverseIfStmt(IfStmt *stmt) override;
  virtual bool TraverseCompoundStmt(CompoundStmt *stmt) override;
#define OPERATOR(NAME)                                                         \
  bool TraverseUnary##NAME(UnaryOperator *E) override;
  UNARYOP_LIST()
#undef OPERATOR
  virtual bool TraverseUnaryOperator(UnaryOperator *op) override;
  virtual bool TraverseUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *E) override;
  virtual bool TraverseBinaryOperator(BinaryOperator *op) override;
#define GENERAL_BINOP_FALLBACK(NAME, BINOP_TYPE)                               \
  bool TraverseBin##NAME(BINOP_TYPE *E) override;
#define OPERATOR(NAME) GENERAL_BINOP_FALLBACK(NAME, BinaryOperator)
  BINOP_LIST()
#undef OPERATOR

#define OPERATOR(NAME)                                                         \
  GENERAL_BINOP_FALLBACK(NAME##Assign, CompoundAssignOperator)
  CAO_LIST()
#undef GENERAL_BINOP_FALLBACK
#undef OPERATOR

  virtual bool TraverseDeclRefExpr(DeclRefExpr *E) override;
  virtual bool TraverseCallExpr(CallExpr *E) override;
  virtual bool TraverseIntegerLiteral(IntegerLiteral *E) override;
  virtual bool TraverseCharacterLiteral(CharacterLiteral *E) override;
  virtual bool TraverseStringLiteral(StringLiteral *E) override;
  bool shouldVisitTemplateInstantiations() const { return true; }
  bool shouldVisitImplicitCode() const { return true; }

private:
  ASTContext *ctx;
  shared_ptr<ProgramNode> prog_;
  shared_ptr<Node> current_node;

  const CommandTraits &Traits;
  const SourceManager &SM;
};

