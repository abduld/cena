

#ifndef __WHILE_H__
#define __WHILE_H__

class WhileNode : public Node {
public:
  WhileNode(const int &row, const int &col) : Node(row, col) {}
  WhileNode(const int &row, const int &col, const shared_ptr<Node> &cond,
            const shared_ptr<Node> &body)
      : Node(row, col), cond_(cond), body_(body) {}
  string getHead() const { return head_; }

private:
  string head_ = "While";
  shared_ptr<Node> cond_;
  shared_ptr<Node> body_;
};
#endif /* __WHILE_H__ */
