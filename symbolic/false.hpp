
#ifndef __FALSE_H__
#define __FALSE_H__

class False : public Keyword {
public:
  False(const int & row, const int & col) : Keyword(row, col, "False") {}
  ~False() {}
};

#endif /* __FALSE_H__ */
