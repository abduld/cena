
#ifndef __BREAK_H__
#define __BREAK_H__

class Break : public Node {
public:
  Break(const int &row, const int &col) : Node(row, col) {}
  Break(const int &row, const int &col, const shared_ptr<Node> &node)
      : Node(row, col), trgt_(node) {}
  ~Break() {}
  string getHead() const { return head_; }
  shared_ptr<Node> getTarget() const { return trgt_; }

private:
  string head_ = "Break";
  shared_ptr<Node> trgt_;
};

#endif /* __BREAK_H__ */