
#ifndef __SIZEOF_H__
#define __SIZEOF_H__

class SizeOfNode : public Node {
public:
  SizeOfNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw)
      : Node(col, row, endrow, endcol, raw) {}
  SizeOfNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const shared_ptr<Node> &arg)
      : Node(row, col, endrow, endcol, raw), arg_(arg) {}
  ~SizeOfNode() {}
  void setArg(const shared_ptr<Node> &arg) { arg_ = arg; }
  shared_ptr<Node> getArg() const { return arg_; }
  string getHead() const override { return head_; }

private:
  string head_ = "SizeOf";
  shared_ptr<Node> arg_;
};

#endif /* __SIZEOF_H__ */
