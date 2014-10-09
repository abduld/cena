

#ifndef __IF_H__
#define __IF_H__

class IfNode : public Node {
public:
  IfNode(const int &row, const int &col) : Node(row, col) {}
  IfNode(const int &row, const int &col, const shared_ptr<Node> &cond,
         const shared_ptr<Node> &thenPart)
      : Node(), cond_(cond), then_(thenPart) {}
  IfNode(const int &row, const int &col, const shared_ptr<Node> &cond,
         const shared_ptr<Node> &thenPart, const shared_ptr<Node> &elsePart)
      : Node(row, col), cond_(cond), then_(thenPart), else_(elsePart) {}
  string getHead() { return head_; }
  void setCondition(const shared_ptr<Node> &cond) { cond_ = cond; }
  void setThen(const shared_ptr<Node> &nd) { then_ = nd; }
  void setElse(const shared_ptr<Node> &nd) { else_ = nd; }
  void toCCode_(ostringstream &o) {
    assert(cond_ != nullptr);
    assert(then_ != nullptr);
    o << "if (";
    cond_->toCCode_(o);
    o << ")";
    then_->toCCode_(o);
    if (else_ != nullptr) {
      o << " else ";
      else_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) {

    assert(cond_ != nullptr);
    assert(then_ != nullptr);
    o << "if (";
    cond_->toString_(o);
    o << ")";
    then_->toString_(o);
    if (else_ != nullptr) {
      o << " else ";
      else_->toString_(o);
    }
  }
  bool isBlock() const { return true; }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }

private:
  string head_ = "If";
  shared_ptr<Node> cond_ = nullptr;
  shared_ptr<Node> then_ = nullptr;
  shared_ptr<Node> else_ = nullptr;
};
#endif /* __IF_H__ */