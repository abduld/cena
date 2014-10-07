
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

class IdentifierNode : public virtual AtomNode<string> {
public:
  IdentifierNode() : AtomNode<string>("unknownid") {}
  IdentifierNode(string v) : AtomNode<string>(v) {}
  IdentifierNode(const shared_ptr<StringNode> &s): AtomNode<string>() {
  	setName(s);
  }
  IdentifierNode(const shared_ptr<Node> &s) : AtomNode<string>(){
  	setName(s);
  }
  void setName(const string & name) {
  	setConstant(name);
  }
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
  string getName() const {
  	return getConstant();
  }
  void setType(const shared_ptr<TypeNode> & typ)  {
  	typ_ = typ;
  }
  shared_ptr<TypeNode> getType()  {
  	return typ_;
  }
  bool isHidden() const {
  	return hidden_;
  }
  void isHidden(bool val)  {
  	hidden_ = val;
  }
  string getHead() { return head_; }

private:
  string head_ = "Identifier";
  shared_ptr<TypeNode> typ_ = nullptr;
  bool hidden_ = false;
};

#endif /* __IDENTIFIER_H__ */
