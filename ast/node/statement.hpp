

#ifndef __STATEMENT_H__
#define __STATEMENT_H__

class Statement : public CompoundNode {
public:
  Statement(const int &row, const int &col, const int &endrow,
            const int &endcol, const string &raw)
      : CompoundNode(row, col, endrow, endcol, raw) {}
  ~Statement() {}
  bool isStatement() const override { return true; }
  string getHead() const override { return head_; }

private:
  string head_ = "Statement";
};

#endif /* __STATEMENT_H__ */
