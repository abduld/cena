

#ifndef __STATEMENT_H__
#define __STATEMENT_H__

class Statement : public CompoundNode {
public:
  Statement() : CompoundNode() {}
  ~Statement() {}
  bool isStatement() const { return true; }
  string getHead() { return head_; }

private:
  string head_ = "Statement";
};

#endif /* __STATEMENT_H__ */
