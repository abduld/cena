
#ifndef __TRUE_H__
#define __TRUE_H__

class True : public Node {
public:
  True() : Node() {}
  ~True() {}
  string getHead() { return head_; }

private:
  string head_ = "True";
};

#endif /* __TRUE_H__ */
