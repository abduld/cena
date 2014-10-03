#include "ast.hpp"

TEST(Statement, CanCreate) {
    shared_ptr<Statement> nd(new Statement());
    ASSERT_EQ(nd->isStatement(), true);
}