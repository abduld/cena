#include "ast.hpp"

TEST(Assign, CanCreate) {
    shared_ptr<Assign> nd(new Assign());
    ASSERT_EQ(nd->isAtom(), false);
    ASSERT_EQ(nd->isStatement(), false);
    nd->setLHS(shared_ptr<Identifier>(new Identifier("v")));
    nd->setRHS(shared_ptr<Integer>(new Integer(2)));
}