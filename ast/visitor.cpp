
#define DEBUG printf("DEBUG :: >>> %s %d ... \n", __PRETTY_FUNCTION__, __LINE__)


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
  bool TraverseUnary##NAME(UnaryOperator *E) { return handleUnaryOperator(E); }

  UNARYOP_LIST()
#undef OPERATOR
  virtual bool TraverseUnaryOperator(UnaryOperator *op) {
    return handleUnaryOperator(op);
  }
  virtual bool TraverseUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *E) {
    DEBUG;
    E->dumpColor();
    current_node = shared_ptr<StringNode>(
        new StringNode(-1, -1, string("TODOXXUnaryExprOrTypeTraitExpr")));
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
        std::cout << "Something has went wrong " << E->getOpcodeStr().str()
                  << std::endl;
        E->dumpColor();
      }
      nd->setRHS(current_node);
      current_node = nd;
    }
    return true;
  }
  virtual bool TraverseBinaryOperator(BinaryOperator *op) {
    return handleBinaryOperator(op);
  }

#define GENERAL_BINOP_FALLBACK(NAME, BINOP_TYPE)                               \
  bool TraverseBin##NAME(BINOP_TYPE *E) { return handleBinaryOperator(E); }
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

  bool shouldVisitTemplateInstantiations() const { return true; }
  bool shouldVisitImplicitCode() const { return true; }

private:
  ASTContext *ctx; // used for getting additional AST info
  shared_ptr<ProgramNode> prog_;
  shared_ptr<Node> current_node;

  const CommandTraits &Traits;
  const SourceManager &SM;
};

