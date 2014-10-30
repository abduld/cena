
#include "ast.hpp"

  void ASTVisitor::visit(const Node * nd) {

  }
  void ASTVisitor::visit(const AssignNode * nd) {
  	if (nd->getLHS()) {
  		visit(nd->getLHS());
  	}
  	if (nd->getRHS()) {
  		visit(nd->getRHS());
  	}
  }