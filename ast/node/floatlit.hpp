

#ifndef __FLOAT_LIT_H__
#define __FLOAT_LIT_H__

class RealNode : public AtomNode<double>, public NodeAcceptor<RealNode> {
public:
  RealNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw)
      : AtomNode<double>(row, col, endrow, endcol, raw) {}
  RealNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw, const double &v)
      : AtomNode<double>(row, col, endrow, endcol, raw, v) {}
  string getHead() const override { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

private:
  string head_ = "Real";
};

#endif /* __FLOAT_LIT_H__ */
