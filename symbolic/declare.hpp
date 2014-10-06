
#ifndef __DECLARE_H__
#define __DECLARE_H__

class DeclareNode : public CompoundNode {
public:
  DeclareNode() : CompoundNode() {}
  void setIdentifier(const shared_ptr<IdentifierNode> & id) {
  	id_ = id;
  }
  void setType(const shared_ptr<TypeNode> & typ) {
  	typ_ = typ;
  }
  void setInitializer(const shared_ptr<Node> & init) {
  	init_ = init;
  }
  string getHead() { return head_; }

private:
  string head_ = "Declare";
  shared_ptr<IdentifierNode> id_;
  shared_ptr<TypeNode> typ_;
  shared_ptr<Node> init_;
};

#endif /* __IDENTIFIER_H__ */
