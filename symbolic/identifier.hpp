
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

class IdentifierNode : public virtual AtomNode<string> {
public:
  IdentifierNode() : AtomNode<string>() {}
  IdentifierNode(string v) : AtomNode<string>(v) {}
  string getHead() { return head_; }

private:
  string head_ = "Identifier";
  TypeNode typ_;
};

#endif /* __IDENTIFIER_H__ */
