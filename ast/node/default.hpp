
#ifndef __DEFAULT_H__
#define __DEFAULT_H__

class Default : public Keyword {
public:
  Default(const int &row, const int &col, const int &endrow, const int &endcol,
          const string &raw)
      : Keyword(row, col, "Default") {}
  ~Default() {}
};

#endif /* __DEFAULT_H__ */
