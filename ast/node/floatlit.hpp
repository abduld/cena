

#ifndef __FLOAT_LIT_H__
#define __FLOAT_LIT_H__

class RealNode : public AtomNode<double> {
public:
  RealNode(const int &row, const int &col) : AtomNode<double>(row, col) {}
  RealNode(const int &row, const int &col, const double &v)
      : AtomNode<double>(row, col, v) {}
  string getHead() { return head_; }
  void traverse(ASTVisitor * visitor) override {
      accept(visitor);
  }

private:
  string head_ = "Real";
};

#endif /* __FLOAT_LIT_H__ */
