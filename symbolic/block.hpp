
#ifndef __BLOCK_H__
#define __BLOCK_H__

class Block : public CompoundNode {
public:
  Block() : CompoundNode() {}
  ~Block() {}
  string getHead() { return head_; }

private:
  string head_ = "Block";
};

#endif /* __BLOCK_H__ */
