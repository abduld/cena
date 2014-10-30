
#ifndef __VISITOR_H__
#define __VISITOR_H__



class ASTVisitor {
public:
  virtual void visit(const Node * nd) = delete;
};

#endif /* __VISITOR_H__ */

