
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

class TypeDef : public Compound {
public:
  TypeDef() : Compound() {}
  TypeDef(Type *lhs, Identifier *rhs) : Compound() {
    push_back(lhs);
    push_back(rhs);
  }
  TypeDef(const shared_ptr<Type> &lhs, const shared_ptr<Identifier> &rhs)
      : Compound() {
    push_back(lhs);
    push_back(rhs);
  }
  ~TypeDef() {}
  void setType(const shared_ptr<Node> &lhs) { setPart(0, lhs); }
  void setName(const shared_ptr<Node> &rhs) { setPart(1, rhs); }
  shared_ptr<Node> getType() { return getPart(0); }
  shared_ptr<Node> getName() { return getPart(1); }
  string getHead() { return head_; }

private:
  string head_ = "TypeDef";
};

#endif /* __TYPEDEF_H__ */
