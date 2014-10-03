#include "ast.hpp"

TEST(Type, CanCreate) {
  shared_ptr<Type> nd(new Type());
  nd->addQualifyer("static");
  nd->addQualifyer("const");
  nd->addBase("int");
}