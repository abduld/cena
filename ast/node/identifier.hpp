
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

class TypeNode;

class IdentifierNode : public virtual AtomNode<string> {
public:
  IdentifierNode(const int &row, const int &col)
      : AtomNode<string>(row, col, "unknownid") {}
  IdentifierNode(const int &row, const int &col, const string &v)
      : AtomNode<string>(row, col, v) {}
  IdentifierNode(const int &row, const int &col,
                 const shared_ptr<StringNode> &s)
      : AtomNode<string>(row, col) {
    setName(s);
  }
  IdentifierNode(const int &row, const int &col, const shared_ptr<Node> &s)
      : AtomNode<string>(row, col) {
    setName(s);
  }
  void setName(const string &name) { setConstant(name); }
  void setName(const shared_ptr<StringNode> &nd) {
    setConstant(nd->getConstant());
  }
  void setName(const shared_ptr<Node> &s) {
    assert(s->isAtom());

    if (s->isAtom()) {
      shared_ptr<StringNode> nd = std::static_pointer_cast<StringNode>(s);
      setConstant(nd->getConstant());
    }
  }
  string getName() const { return getConstant(); }
  void setType(const shared_ptr<TypeNode> &typ) { typ_ = typ; }
  shared_ptr<TypeNode> getType() const { return typ_; }
  bool isHidden() const { return hidden_; }
  void isHidden(bool val) { hidden_ = val; }
  string getHead() const { return head_; }

  virtual void toCCode_(ostringstream &o) { o << getConstant(); }
  virtual void toString_(ostringstream &o) { o << getConstant(); }
  virtual void toJSON_(ostringstream &o) { toCCode_(o); }
  virtual void toEsprima_(ostringstream &o) {
    o << "{\"type\": \"Literal\", \"value\": ";
    toCCode_(o);
    o << "}";
  }

private:
  string head_ = "Identifier";
  shared_ptr<TypeNode> typ_ = nullptr;
  bool hidden_ = false;
};

#endif /* __IDENTIFIER_H__ */
