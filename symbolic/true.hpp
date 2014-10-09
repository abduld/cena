
#ifndef __TRUE_H__
#define __TRUE_H__

class True : public Keyword {
public:
  True(const int &row, const int &col) : Keyword(row, col, "True") {}
  ~True() {}
};

#endif /* __TRUE_H__ */
