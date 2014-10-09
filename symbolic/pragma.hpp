
#ifndef __PRAGMA_H__
#define __PRAGMA_H__

class Pragma : public CompoundNode {
public:
  Pragma(const int & row, const int & col, const shared_ptr<Node> &node) : CompoundNode(row, col) { push_back(node); }
  ~Pragma() {}
  string getHead() { return head_; }

private:
  string head_ = "Pragma";
};

#endif /* __PRAGMA_H__ */
