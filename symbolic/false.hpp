
#ifndef __FALSE_H__
#define __FALSE_H__

class False : public Compound {
public:
  False() : Compound() {}
  ~False() {}
  string getHead() { return head_; }

private:
  string head_ = "False";
};

#endif /* __FALSE_H__ */
