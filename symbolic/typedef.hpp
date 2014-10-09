
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

class TypeDefNode : public CompoundNode {
public:
  TypeDefNode() : CompoundNode() {}
  TypeDefNode(TypeNode *lhs, IdentifierNode *rhs) : CompoundNode() {
    push_back(lhs);
    push_back(rhs);
  }
  TypeDefNode(const shared_ptr<TypeNode> &lhs,
              const shared_ptr<IdentifierNode> &rhs)
      : CompoundNode() {
    push_back(lhs);
    push_back(rhs);
  }
  ~TypeDefNode() {}
  void setTypeNode(const shared_ptr<Node> &lhs) { setPart(0, lhs); }
  void setName(const shared_ptr<Node> &rhs) { setPart(1, rhs); }
  shared_ptr<Node> getTypeNode() const { return getPart(0); }
  shared_ptr<Node> getName() const { return getPart(1); }
  string getHead() const { return head_; }

private:
  string head_ = "TypeDef";
};

#endif /* __TYPEDEF_H__ */
