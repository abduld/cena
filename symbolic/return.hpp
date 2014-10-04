
#ifndef __RETURN_H__
#define __RETURN_H__

class Return : public CompoundNode {
public:
  Return() : CompoundNode() {}
  Return(Node *nd) : CompoundNode() { push_back(nd); }
  Return(const shared_ptr<Node> &nd) : CompoundNode() { push_back(nd); }
  ~Return() {}
  void setArg(const shared_ptr<Node> &arg) { setPart(0, arg); }
  shared_ptr<Node> getArg() { return getPart(0); }
  string getHead() { return head_; }

private:
  string head_ = "Return";
};

#endif /* __RETURN_H__ */
