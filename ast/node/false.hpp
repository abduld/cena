
#ifndef __FALSE_H__
#define __FALSE_H__

class False : public Keyword {
public:
  False(const int &row, const int &col, const int &endrow, const int &endcol,
        const string &raw)
      : Keyword(row, col, endrow, endcol, raw, "False") {}
  ~False() {}
};

#endif /* __FALSE_H__ */
