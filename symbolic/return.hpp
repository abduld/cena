
#ifndef __RETURN_H__
#define __RETURN_H__

class ReturnNode : public Node {
public:
  ReturnNode() : Node() {}
  ReturnNode(const shared_ptr<Node> &nd) : Node() { setReturnValue(nd); }
  ~ReturnNode() {}
  void setReturnValue(const shared_ptr<Node> &arg) { ret_ = arg; }
  shared_ptr<Node> getReturnValue() { return ret_; }
  string getHead() { return head_; }
  bool isStatement() const { return true; }
  void toCCode_(ostringstream &o) {
    o << "return";
    if (ret_ != nullptr) {
      o << " ";
      ret_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) {
    o << "return";
    if (ret_ != nullptr) {
      o << " ";
      ret_->toString_(o);
    }
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }

private:
  string head_ = "Return";
  shared_ptr<Node> ret_ = nullptr;
};

#endif /* __RETURN_H__ */
