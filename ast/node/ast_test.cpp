#include "ast.hpp"

using ::testing::InitGoogleTest;
using namespace std;

int main(int argc, char **argv) {
  int res;

  // Initialize Google's testing library
  InitGoogleTest(&argc, argv);
#if USING_VS_COMPILER
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif /* USING_VS_COMPILER */

  res = RUN_ALL_TESTS();

  return res;
}