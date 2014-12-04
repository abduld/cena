
#ifndef __GOTO_H__
#define __GOTO_H__

class Goto : public Node {
public:
  Goto(const int &row, const int &col, const shared_ptr<Node> &node)
      : Node(row, col, endrow, endcol, raw), trgt_(trgt) {}
  ~Goto() {}
  string getHead() { return head_; }

private:
  string head_ = "Goto";
  shared_ptr<Node> trgt_;
};

#endif /* __GOTO_H__ */
