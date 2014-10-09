

#ifndef __STATEMENT_H__
#define __STATEMENT_H__

class Statement : public CompoundNode {
public:
  Statement(const int &row, const int &col) : CompoundNode(row, col) {}
  ~Statement() {}
  bool isStatement() const { return true; }
  string getHead() const { return head_; }

private:
  string head_ = "Statement";
};

#endif /* __STATEMENT_H__ */
