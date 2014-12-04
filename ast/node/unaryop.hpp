
#ifndef __UNARY_OP_H__
#define __UNARY_OP_H__

class UnaryOp : public SymbolNode {
public:
  UnaryOp(const int &row, const int &col, const int &endrow, const int &endcol,
          const string &raw, const char *s)
      : SymbolNode(row, col, endrow, endcol, raw, s) {}
  UnaryOp(const int &row, const int &col, const int &endrow, const int &endcol,
          const string &raw, const string &s)
      : SymbolNode(row, col, endrow, endcol, raw, s) {}
  string getHead() const { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

private:
  string head_ = "UnaryOperator";
};

class UnaryOperatorNode : public Node {
public:
  UnaryOperatorNode(const int &row, const int &col,const int &endrow, const int &endcol,
          const string &raw) : Node(row, col, endrow, endcol, raw) {}
  UnaryOperatorNode(const int &row, const int &col,const int &endrow, const int &endcol,
          const string &raw, const string &op,
                    const shared_ptr<Node> &arg)
      : Node(row, col, endrow, endcol, raw), op_(shared_ptr<UnaryOp>(new UnaryOp(row, col, endrow, endcol, raw, op))),
        arg_(arg) {}
  ~UnaryOperatorNode() {}
  void setOperator(const string &op) {
    op_ = shared_ptr<UnaryOp>(new UnaryOp(row_, col_, endrow_, endcol_, op, op));
  }
  void setArg(const shared_ptr<Node> &arg) { arg_ = arg; }

  shared_ptr<UnaryOp> getOperator() const { return op_; }
  shared_ptr<Node> getArg() const { return arg_; }
  string getHead() const { return head_; }

  void toCCode_(ostringstream &o) {
    assert(arg_ != nullptr);
    assert(op_ != nullptr);
    op_->toCCode_(o);
    o << " ";
    arg_->toCCode_(o);
  }
  void toString_(ostringstream &o) {
    assert(arg_ != nullptr);
    assert(op_ != nullptr);
    op_->toString_(o);
    o << " ";
    arg_->toString_(o);
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "UnaryExpression";
    obj["prefix"] = true;
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["operator"] = op_->toString();
    obj["argument"] = arg_->toEsprima();
    return obj;
  }
  void traverse(ASTVisitor *visitor) override {
    op_->traverse(visitor);
    arg_->traverse(visitor);
  }

private:
  string head_ = "UnaryOperator";
  shared_ptr<UnaryOp> op_ = nullptr;
  shared_ptr<Node> arg_ = nullptr;
};

#endif /* __UNARY_OP_H__ */
