
#ifndef __TRUE_H__
#define __TRUE_H__

class True : public Compound {
public:
  True() : Compound() {}
  ~True() {}
  string getHead() { return head_; }

private:
  string head_ = "True";
};

#endif /* __TRUE_H__ */
