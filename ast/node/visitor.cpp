
#include "ast.hpp"

void ASTVisitor::visit(const Node *nd) {}
void ASTVisitor::visit(const AssignNode *nd) {
  if (nd->getLHS()) {
    nd->getLHS()->accept(this);
  }
  if (nd->getRHS()) {
    nd->getRHS()->accept(this);
  }
}
