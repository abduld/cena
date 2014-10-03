

#ifndef __PROGRAM_H__
#define __PROGRAM_H__

class Program : public Compound {
public:
  Program() : Compound() {}
  ~Program() {}
  string getHead() { return head_; }

private:
  string head_ = "Program";
};

#endif /* __PROGRAM_H__ */
