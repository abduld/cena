

#ifndef __DEREFERENCE_H__
#define __DEREFERENCE_H__

class Dereference : public Node {
public:
  Dereference() : Node() {}
  Dereference(const shared_ptr<Node> &typ) : Node() { type_ = typ; }
  string getHead() const { return head_; }

private:
  string head_ = "Dereference";
  shared_ptr<Node> type_;
};
#endif /* __DEREFERENCE_H__ */
