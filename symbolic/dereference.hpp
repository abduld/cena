

#ifndef __DEREFERENCE_H__
#define __DEREFERENCE_H__

class Dereference : public Node {
public:
  Dereference(const int &row, const int &col) : Node(row, col) {}
  Dereference(const int &row, const int &col, const shared_ptr<Node> &typ)
      : Node(row, col), typ_(typ) {}
  string getHead() const { return head_; }

private:
  string head_ = "Dereference";
  shared_ptr<Node> typ_;
};
#endif /* __DEREFERENCE_H__ */
