
#ifndef __BREAK_H__
#define __BREAK_H__

class Break : public CompoundNode {
public:
  Break() : CompoundNode() {}
  Break(const shared_ptr<Node> &node) : CompoundNode() { push_back(node); }
  ~Break() {}
  string getHead() { return head_; }

private:
  string head_ = "Break";
};

#endif /* __BREAK_H__ */
