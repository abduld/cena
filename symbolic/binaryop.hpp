
#ifndef __BINARY_OP_H__
#define __BINARY_OP_H__

class BinaryOp : public StringNode {
public:
  BinaryOp(const char *s) : StringNode(s) {}
  BinaryOp(string s) : StringNode(s) {}
  string getHead() { return head_; }

private:
  string head_ = "BinaryOp";
};

class BinaryOperatorNode : public CompoundNode {
public:
  BinaryOperatorNode() : CompoundNode() {}
  BinaryOperatorNode(string op, Node *lhs, Node *rhs) : CompoundNode() {
    push_back(shared_ptr<BinaryOp>(new BinaryOp(op)));
    push_back(lhs);
    push_back(rhs);
  }
  BinaryOperatorNode(string op, const shared_ptr<Node> &lhs,
                     const shared_ptr<Node> &rhs)
      : CompoundNode() {
    push_back(shared_ptr<BinaryOp>(new BinaryOp(op)));
    push_back(lhs);
    push_back(rhs);
  }
  ~BinaryOperatorNode() {}
  void setOperator(const shared_ptr<Node> &op) { setPart(0, op); }
  void setLHS(const shared_ptr<Node> &lhs) { setPart(1, lhs); }

  void setRHS(const shared_ptr<Node> &rhs) { setPart(2, rhs); }
  shared_ptr<Node> getOperator() { return getPart(0); }
  shared_ptr<Node> getLHS() { return getPart(1); }
  shared_ptr<Node> setRHS() { return getPart(2); }
  string getHead() { return head_; }

private:
  string head_ = "BinaryOperatorNode";
};

#endif /* __BINARY_OP_H__ */
