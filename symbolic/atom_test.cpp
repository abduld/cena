#include "ast.hpp"

TEST(BooleanNode, CanCreate) {
  shared_ptr<BooleanNode> nd(new BooleanNode(true));
  ASSERT_STREQ(nd->toString().c_str(), "true");
  ASSERT_STREQ(nd->getHead().c_str(), "Boolean");
}
TEST(IntegerNode, CanCreate) {
  shared_ptr<IntegerNode> nd(new IntegerNode(1));
  ASSERT_STREQ(nd->toString().c_str(), "1");
  ASSERT_STREQ(nd->getHead().c_str(), "Integer");
}
TEST(RealNode, CanCreate) {
  shared_ptr<RealNode> nd(new RealNode(2.1));
  ASSERT_STREQ(nd->toString().c_str(), "2.1");
  ASSERT_STREQ(nd->getHead().c_str(), "Real");
}
TEST(String, CanCreate) {
  shared_ptr<StringNode> nd(new StringNode("foo"));
  ASSERT_STREQ(nd->toString().c_str(), "foo");
  ASSERT_STREQ(nd->getHead().c_str(), "String");
}
