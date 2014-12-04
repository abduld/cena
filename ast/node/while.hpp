

#ifndef __WHILE_H__
#define __WHILE_H__

class WhileNode : public Node {
public:
  WhileNode(const int &row, const int &col, const int &endrow,
            const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  WhileNode(const int &row, const int &col, const int &endrow,
            const int &endcol, const string &raw, const shared_ptr<Node> &cond,
            const shared_ptr<Node> &body)
      : Node(row, col, endrow, endcol, raw), cond_(cond), body_(body) {}
  string getHead() const { return head_; }

private:
  string head_ = "While";
  shared_ptr<Node> cond_;
  shared_ptr<Node> body_;
};
#endif /* __WHILE_H__ */
