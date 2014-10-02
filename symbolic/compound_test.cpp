#include "ast.hpp"

TEST(Compound, CanCreate) {
  shared_ptr<Compound> nd(new Compound());
  ASSERT_STREQ(nd->getHead().c_str(), "Compound");
}
TEST(Compound, CanAdd) {
  shared_ptr<Compound> nd(new Compound());
  *nd <<= 99.0;
  *nd <<= 3;
  vector<shared_ptr<Node> > vals = nd->getValues();
  shared_ptr<Node> real_val = vals[0];
  shared_ptr<Node> int_val = vals[1];
  ASSERT_STREQ(real_val->getHead().c_str(), "Real");
  ASSERT_STREQ(int_val->getHead().c_str(), "Integer");
}