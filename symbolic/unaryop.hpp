
#ifndef __UNARY_OP_H__
#define __UNARY_OP_H__

class UnaryOp : public StringNode {
public:
  UnaryOp(const int &row, const int &col, const char *s)
      : StringNode(row, col, s) {}
  UnaryOp(const int &row, const int &col, const string &s)
      : StringNode(row, col, s) {}
  string getHead() const { return head_; }

private:
  string head_ = "UnaryOperator";
};

class UnaryOperatorNode : public Node {
public:
  UnaryOperatorNode(const int &row, const int &col) : Node(row, col) {}
  UnaryOperatorNode(const int &row, const int &col, const string &op,
                    const shared_ptr<Node> &arg)
      : Node(row, col), op_(shared_ptr<UnaryOp>(new UnaryOp(row, col, op))),
        arg_(arg) {}
  ~UnaryOperatorNode() {}
  void setOperator(const string &op) {
    op_ = shared_ptr<UnaryOp>(new UnaryOp(row_, col_, op));
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

private:
  string head_ = "UnaryOperator";
  shared_ptr<UnaryOp> op_ = nullptr;
  shared_ptr<Node> arg_ = nullptr;
};

#endif /* __UNARY_OP_H__ */
