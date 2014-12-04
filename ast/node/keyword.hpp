
#ifndef __KEYWORD_H__
#define __KEYWORD_H__

class Keyword : public Node {
public:
  Keyword(const int &row, const int &col, const int &endrow, const int &endcol,
          const string &raw, const string &head)
      : Node(row, col, endrow, endcol, raw), head_(head) {}
  ~Keyword() {}
  string getHead() { return head_; }

protected:
  string head_;
};

#endif /* __KEYWORD_H__ */
