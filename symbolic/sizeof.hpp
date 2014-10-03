
#ifndef __SIZEOF_H__
#define __SIZEOF_H__

class SizeOf : public Compound {
public:
  SizeOf() : Compound() {}
  SizeOf(Node *nd) : Compound() {
    push_back(nd);
  }
  SizeOf(const shared_ptr<Node> &nd)
      : Compound() {
    push_back(nd);
  }
  ~TypeDef() {}
  void setArg(const shared_ptr<Node> &arg) { setPart(0, lhs); }
  shared_ptr<Node> getArg() { return getPart(0); }
  string getHead() { return head_; }

private:
  string head_ = "SizeOf";
};

#endif /* __SIZEOF_H__ */
