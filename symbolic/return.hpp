
#ifndef __RETURN_H__
#define __RETURN_H__

class Return : public Compound {
public:
  Return() : Compound() {}
  Return(Node *nd) : Compound() {
    push_back(nd);
  }
  Return(const shared_ptr<Node> &nd)
      : Compound() {
    push_back(nd);
  }
  ~Return() {}
  void setArg(const shared_ptr<Node> &arg) { setPart(0, lhs); }
  shared_ptr<Node> getArg() { return getPart(0); }
  string getHead() { return head_; }

private:
  string head_ = "Return";
};

#endif /* __RETURN_H__ */
