#include "ast.hpp"

TEST(CompoundNode, CanCreate) {
  shared_ptr<CompoundNode> nd(new CompoundNode());
  ASSERT_STREQ(nd->getHead().c_str(), "CompoundNode");
}
TEST(CompoundNode, CanAdd) {
  shared_ptr<CompoundNode> nd(new CompoundNode());
  *nd <<= 99.0;
  *nd <<= 3;
  vector<shared_ptr<Node> > vals = nd->getValues();
  shared_ptr<Node> real_val = vals[0];
  shared_ptr<Node> int_val = vals[1];
  ASSERT_STREQ(real_val->getHead().c_str(), "Real");
  ASSERT_STREQ(int_val->getHead().c_str(), "Integer");
}