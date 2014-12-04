

#ifndef __LEVENSHTEIN_H__
#define __LEVENSHTEIN_H__

#include <string>
#include <algorithm>
#include <vector>

using std::vector;
using std::string;

static int levenshtein_distance0(const char *a, const size_t &a_len,
                                 const char *b, const size_t &b_len) {
  int cost = 0;
  if (a_len == 0) {
    return b_len;
  } else if (b_len == 0) {
    return a_len;
  }
  if (a[a_len - 1] != b[b_len - 1]) {
    cost = 1;
  }
  return std::min(
      levenshtein_distance0(a, a_len - 1, b, b_len) + 1,
      std::min(levenshtein_distance0(a, a_len, b, b_len - 1) + 1,
               levenshtein_distance0(a, a_len - 1, b, b_len - 1) + cost));
}

static int levenshtein_distance1(const char *s, const size_t &m, const char *t,
                                 const size_t &n) {
  int d[m + 1][n + 1];
  for (int ii = 1; ii < m; ii++) {
    d[ii][0] = 1;
    for (int jj = 1; jj < n; jj++) {
      d[ii][jj] = 0;
    }
  }
  for (int jj = 0; jj < n; jj++) {
    d[0][jj] = jj;
  }
  for (int jj = 1; jj < n; jj++) {
    for (int ii = 1; ii < m; ii++) {
      if (s[ii] == t[jj]) {
        d[ii][jj] = d[ii - 1][jj - 1];
      } else {
        d[ii][jj] =
            std::min(d[ii - 1][jj] + 1,
                     std::min(d[ii][jj - 1] + 1, d[ii - 1][jj - 1] + 1));
      }
    }
  }
  return d[m][n];
}

static int levenshtein_distance(const char *s, const size_t &m, const char *t,
                                const size_t &n) {

  vector<int> prev(n + 1);
  vector<int> curr(n + 1);
  for (int ii = 1; ii <= n; ii++) {
    prev[ii] = ii;
  }
  for (int ii = 0; ii <= m; ii++) {
    curr[0] = ii + 1;
    for (int jj = 0; jj < n; jj++) {
      int cost = s[ii] == t[jj] ? 0 : 1;
      // std::cout << s[ii] << " =  " << t[jj] << " " << cost << std::endl;
      curr[jj + 1] = std::min(std::min(curr[jj] + 1 // deletion
                                       ,
                                       prev[jj + 1] + 1) // insersion
                              ,
                              prev[jj] + cost // substitution
                              );
    }
    curr.swap(prev);
    /*
  for (int jj = 0; jj <= n; jj++) {
      std::cout << "curr[" << jj << "] = " << curr[jj] << std::endl;
    }
    */
  }
  return curr[n];
}
static int levenshtein_distance(const char *a, const char *b) {
  if (a == b) {
    return 0;
  } else if (a == NULL) {
    return strlen(b);
  } else if (b == NULL) {
    return strlen(a);
  }
  size_t a_len = strlen(a);
  size_t b_len = strlen(b);
  if (a_len == 0 || b_len == 0) {
    return std::max(a_len, b_len);
  } else if (a_len == b_len && memcmp(a, b, a_len) == 0) {
    return 0;
  }
  return levenshtein_distance(a, a_len, b, b_len);
}
static int levenshtein_distance(const string &a, const string &b) {
  return levenshtein_distance(a.c_str(), b.c_str());
}

#endif /* __LEVENSHTEIN_H__ */
