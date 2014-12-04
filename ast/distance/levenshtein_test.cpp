
#include <iostream>
#include "levenshtein.hpp"

#define TEST0(a, b, expected)                                                  \
  {                                                                            \
    int dist = levenshtein_distance(a, b);                                     \
    if (dist != expected) {                                                    \
      std::cerr << "TEST FAILED:: Distance(" << a << ", " << b                 \
                << ") = " << dist << " and not " << expected << std::endl;     \
    } else {                                                                   \
      std::cout << "Passed Distance(" << a << ", " << b << ") = " << expected  \
                << std::endl;                                                  \
    }                                                                          \
  }
#define TEST(a, b, expected) TEST0(string(a), string(b), expected)
int main() {

  TEST("", "", 0);
  TEST("a", "", 1);
  TEST("", "a", 1);
  TEST("abc", "", 3);
  TEST("", "abc", 3);
  TEST("abc", "ac", 1);
  TEST("ac", "abc", 1);
  TEST("abc", "abc", 0);
  TEST("abc", "axc", 1);
  TEST("abc", "axxxx", 4);
  TEST("axxxx", "abc", 4);
  TEST("example", "samples", 3);
  TEST("levenshtein", "frankenstein", 6);
  TEST("xabxcdxxefxgx", "1ab2cd34ef5g6", 6);
  return 0;
}
