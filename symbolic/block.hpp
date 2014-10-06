
#ifndef __BLOCK_H__
#define __BLOCK_H__

class BlockNode : public CompoundNode {
public:
  BlockNode() : CompoundNode() {}
  ~BlockNode() {}
  string getHead() { return head_; }

private:
  string head_ = "Block";
};

#endif /* __BLOCK_H__ */
