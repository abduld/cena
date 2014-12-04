
#ifndef __TRUE_H__
#define __TRUE_H__

class True : public Keyword {
public:
  True(const int &row, const int &col, const int &endrow, const int &endcol,
       const string &raw)
      : Keyword(row, col, endrow, endcol, raw, "True") {}
  ~True() {}
};

#endif /* __TRUE_H__ */
