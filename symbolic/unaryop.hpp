
#ifndef __UNARY_OP_H__
#define __UNARY_OP_H__

class UnaryOp : public StringNode {
public:
  UnaryOp(const char *s) : StringNode(s) {}
  UnaryOp(const string &s) : StringNode(s) {}
  string getHead() { return head_; }

private:
  string head_ = "UnaryOperator";
};

class UnaryOperatorNode : public Node {
public:
  UnaryOperatorNode() : Node() {}
  UnaryOperatorNode(const string &op, const shared_ptr<Node> &arg)
      : Node(), op_(shared_ptr<UnaryOp>(new UnaryOp(op))), arg_(arg) {}
  ~UnaryOperatorNode() {}
  void setOperator(const string &op) {
    op_ = shared_ptr<UnaryOp>(new UnaryOp(op));
  }
  void setArg(const shared_ptr<Node> &arg) { arg_ = arg; }

  shared_ptr<UnaryOp> getOperator() { return op_; }
  shared_ptr<Node> getArg() { return arg_; }
  string getHead() { return head_; }

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

private:
  string head_ = "UnaryOperator";
  shared_ptr<UnaryOp> op_ = nullptr;
  shared_ptr<Node> arg_ = nullptr;
};

#endif /* __UNARY_OP_H__ */
