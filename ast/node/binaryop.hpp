
#ifndef __BINARY_OP_H__
#define __BINARY_OP_H__

class BinaryOp : public StringNode, NodeAcceptor<BinaryOp> {
public:
  BinaryOp(const int &row, const int &col, const char *s)
      : StringNode(row, col, s) {}
  BinaryOp(const int &row, const int &col, const string &s)
      : StringNode(row, col, s) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) { o << getConstant(); }
  void toString_(ostringstream &o) { toCCode_(o); }

private:
  string head_ = "BinaryOp";
};

class BinaryOperatorNode : public Node, NodeAcceptor<BinaryOperatorNode> {
public:
  BinaryOperatorNode(const int &row, const int &col) : Node(row, col) {}
  BinaryOperatorNode(const int &row, const int &col, const string &op,
                     const shared_ptr<Node> &lhs, const shared_ptr<Node> &rhs)
      : Node(row, col) {
    op_ = shared_ptr<BinaryOp>(new BinaryOp(row_, col_, op));
    lhs_ = lhs;
    rhs_ = rhs;
  }
  ~BinaryOperatorNode() {}
  void setOperator(const string &op) {
    op_ = shared_ptr<BinaryOp>(new BinaryOp(row_, col_, op));
  }
  void setOperator(const shared_ptr<BinaryOp> &op) { op_ = op; }
  void setLHS(const shared_ptr<Node> &lhs) { lhs_ = lhs; }

  void setRHS(const shared_ptr<Node> &rhs) { rhs_ = rhs; }
  shared_ptr<Node> getOperator() const { return op_; }
  shared_ptr<Node> getLHS() const { return lhs_; }
  shared_ptr<Node> setRHS() const { return rhs_; }
  string getHead() { return head_; }

  void toCCode_(ostringstream &o) {
    assert(lhs_ != nullptr);
    assert(rhs_ != nullptr);
    assert(op_ != nullptr);
    lhs_->toCCode_(o);
    o << " ";
    op_->toCCode_(o);
    o << " ";
    rhs_->toCCode_(o);
  }
  void toString_(ostringstream &o) {
    assert(lhs_ != nullptr);
    assert(rhs_ != nullptr);
    assert(op_ != nullptr);
    lhs_->toString_(o);
    o << " ";
    op_->toString_(o);
    o << " ";
    rhs_->toString_(o);
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "BinaryExpression";
    obj["line"] = row_;
    obj["column"] = col_;
    obj["left"] = lhs_->toEsprima_();
    obj["right"] = rhs_->toEsprima_();
    obj["operator"] = op_->toString();
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }

  bool hasChildren() const override { return lhs_ != nullptr || rhs_ != nullptr; }
vector<shared_ptr<Node> > getChildren() override {
  assert(op_ != nullptr);
  if (hasChildren() == false) {
    return vector<shared_ptr<Node> > {};
  } else if (lhs_ != nullptr && rhs_ != nullptr) {
    return vector<shared_ptr<Node> > {lhs_, op_, rhs_};
  } else if (lhs_ != nullptr ) {
    return vector<shared_ptr<Node> > {lhs_, op_};
  } else if (rhs_ != nullptr) {
    return vector<shared_ptr<Node> > {rhs_, op_};
  } else {
    assert(false);
    return vector<shared_ptr<Node> > {};
  }
}
private:
  string head_ = "BinaryOperatorNode";
  shared_ptr<BinaryOp> op_ = nullptr;
  shared_ptr<Node> lhs_ = nullptr;
  shared_ptr<Node> rhs_ = nullptr;
};

#endif /* __BINARY_OP_H__ */
