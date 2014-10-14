
#ifndef __BLOCK_H__
#define __BLOCK_H__

class BlockNode : public CompoundNode {
public:
  BlockNode(const int &row, const int &col) : CompoundNode(row, col) {}
  ~BlockNode() {}
  string getHead() const { return head_; }
  bool isBlock() const { return true; }

private:
  string head_ = "Block";
};

#endif /* __BLOCK_H__ */
