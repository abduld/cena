
#ifndef __INTEGER_LIT_H__
#define __INTEGER_LIT_H__

class IntegerNode : public virtual AtomNode<int64_t> {
public:
  IntegerNode(const int &row, const int &col) : AtomNode<int64_t>(row, col) {}
  IntegerNode(const int &row, const int &col, const int64_t &v)
      : AtomNode<int64_t>(row, col, v) {}
  string getHead() const { return head_; }
  void traverse(ASTVisitor * visitor) override {
      accept(visitor);
  }

private:
  string head_ = "Integer";
};

#endif /* __INTEGER_LIT_H__ */
