#include "ast.hpp"

TEST(Type, CanCreate) {
  shared_ptr<TypeNode> nd(new TypeNode());
  nd->addQualifyer("static");
  nd->addQualifyer("const");
  nd->addBase("int");
}
