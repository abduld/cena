
#ifndef __VISITOR_H__
#define __VISITOR_H__

class ASTVisitor {
public:
  virtual void visit(const Node *nd) {}
  virtual void visit(const AssignNode *nd) {}
  virtual void visit(const CharacterNode *nd) {}
  virtual void visit(const IntegerNode *nd) {}
  virtual void visit(const RealNode *nd) {}
  virtual void visit(const StringNode *nd) {}
  virtual void visit(const SymbolNode *nd) {}
  virtual void visit(const UnaryOp *nd) {}
};

template <typename Derived> class NodeAcceptor {
public:
  virtual void accept(ASTVisitor *visitor) {
    visitor->visit(static_cast<Derived *>(this));
  }
  virtual void traverse(ASTVisitor *visitor){};
};

#endif /* __VISITOR_H__ */
