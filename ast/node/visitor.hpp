
#ifndef __VISITOR_H__
#define __VISITOR_H__

class ASTVisitor {
public:
  virtual void visit(const Node *nd);
  virtual void visit(const AssignNode *nd);
};

template <typename Derived> class NodeAcceptor {
public:
  void accept(ASTVisitor *visitor) {
    visitor->visit(static_cast<Derived *>(this));
  }
  virtual void traverse(ASTVisitor *visitor) = delete;
};

#endif /* __VISITOR_H__ */
