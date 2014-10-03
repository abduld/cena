
#ifndef __FALSE_H__
#define __FALSE_H__

class False : public Node {
public:
  False() : Node() {}
  ~False() {}
  string getHead() { return head_; }

private:
  string head_ = "False";
};

#endif /* __FALSE_H__ */
