
#ifndef __SIZEOF_H__
#define __SIZEOF_H__

class SizeOf : public CompoundNode {
public:
  SizeOf() : CompoundNode() {}
  SizeOf(Node *nd) : CompoundNode() { push_back(nd); }
  SizeOf(const shared_ptr<Node> &nd) : CompoundNode() { push_back(nd); }
  ~SizeOf() {}
  void setArg(const shared_ptr<Node> &arg) { setPart(0, arg); }
  shared_ptr<Node> getArg() { return getPart(0); }
  string getHead() { return head_; }

private:
  string head_ = "SizeOf";
};

#endif /* __SIZEOF_H__ */
