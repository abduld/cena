
#ifndef __SIZEOF_H__
#define __SIZEOF_H__

class SizeOf : public Node {
public:
  SizeOf(const int &row, const int &col) : Node(col, row) {}
  SizeOf(const int &row, const int &col, const shared_ptr<Node> &arg)
      : Node(row, col), arg_(arg) {}
  ~SizeOf() {}
  void setArg(const shared_ptr<Node> &arg) { arg_ = arg; }
  shared_ptr<Node> getArg() const { return arg_; }
  string getHead() const { return head_; }

private:
  string head_ = "SizeOf";
  shared_ptr<Node> arg_;
};

#endif /* __SIZEOF_H__ */
