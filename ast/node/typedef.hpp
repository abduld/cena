
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

class TypeDefNode : public Node {
public:
  TypeDefNode(const int &row, const int &col) : Node(row, col) {}
  TypeDefNode(const int &row, const int &col, const shared_ptr<TypeNode> &lhs,
              const shared_ptr<IdentifierNode> &rhs)
      : Node(row, col), type_(lhs), alias_(rhs) {}
  ~TypeDefNode() {}
  void setTypeNode(const shared_ptr<TypeNode> &lhs) { type_ = lhs; }
  void setName(const shared_ptr<IdentifierNode> &rhs) { alias_ = rhs; }
  shared_ptr<Node> getTypeNode() const { return type_; }
  shared_ptr<Node> getName() const { return alias_; }
  string getHead() const { return head_; }

private:
  string head_ = "TypeDef";
  shared_ptr<TypeNode> type_;
  shared_ptr<IdentifierNode> alias_;
};

#endif /* __TYPEDEF_H__ */
