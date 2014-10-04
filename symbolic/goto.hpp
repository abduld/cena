
#ifndef __GOTO_H__
#define __GOTO_H__

class Goto : public CompoundNode {
public:
  Goto(const shared_ptr<Node> &node) : CompoundNode() { push_back(node); }
  ~Goto() {}
  string getHead() { return head_; }

private:
  string head_ = "Goto";
};

#endif /* __GOTO_H__ */
