
#ifndef __INTEGER_LIT_H__
#define __INTEGER_LIT_H__

class IntegerNode : public virtual AtomNode<int64_t>,
                    public NodeAcceptor<IntegerNode> {
public:
  IntegerNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<int64_t>(row, col, endrow, endcol, raw) {}
  IntegerNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const int64_t &v)
      : AtomNode<int64_t>(row, col, endrow, endcol, raw, v) {}
  string getHead() const { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

private:
  string head_ = "Integer";
};

#endif /* __INTEGER_LIT_H__ */
