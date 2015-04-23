

#ifndef __DEREFERENCE_H__
#define __DEREFERENCE_H__

class Dereference : public Node {
public:
  Dereference(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  Dereference(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const shared_ptr<Node> &typ)
      : Node(row, col, endrow, endcol, raw), typ_(typ) {}
  string getHead() const override { return head_; }

private:
  string head_ = "Dereference";
  shared_ptr<Node> typ_;
};
#endif /* __DEREFERENCE_H__ */
