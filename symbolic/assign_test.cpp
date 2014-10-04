#include "ast.hpp"

TEST(Assign, CanCreate) {
  shared_ptr<AssignNode> nd(new AssignNode());
  ASSERT_EQ(nd->isAtom(), false);
  ASSERT_EQ(nd->isStatement(), false);
  nd->setLHS(shared_ptr<IdentifierNode>(new IdentifierNode("v")));
  nd->setRHS(shared_ptr<IntegerNode>(new IntegerNode(2)));
}
