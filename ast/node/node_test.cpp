#include "ast.hpp"

TEST(Node, CanCreate) {
  shared_ptr<Node> nd(new Node());
  ASSERT_STREQ(nd->toString().c_str(), "unknown");
  ASSERT_STREQ(nd->getHead().c_str(), "unknown");
  ASSERT_TRUE(nd->getValues().size() == 0);
}