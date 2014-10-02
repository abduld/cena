#include "ast.hpp"

TEST(Boolean, CanCreate) {
  shared_ptr<Boolean> nd(new Boolean(true));
  ASSERT_STREQ(nd->toString().c_str(), "true");
  ASSERT_STREQ(nd->getHead().c_str(), "Boolean");
}
TEST(Integer, CanCreate) {
  shared_ptr<Integer> nd(new Integer(1));
  ASSERT_STREQ(nd->toString().c_str(), "1");
  ASSERT_STREQ(nd->getHead().c_str(), "Integer");
}
TEST(Real, CanCreate) {
  shared_ptr<Real> nd(new Real(2.1));
  ASSERT_STREQ(nd->toString().c_str(), "2.1");
  ASSERT_STREQ(nd->getHead().c_str(), "Real");
}
TEST(String, CanCreate) {
  shared_ptr<String> nd(new String("foo"));
  ASSERT_STREQ(nd->toString().c_str(), "foo");
  ASSERT_STREQ(nd->getHead().c_str(), "String");
}