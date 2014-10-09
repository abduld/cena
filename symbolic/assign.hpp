
#ifndef __ASSIGN_H__
#define __ASSIGN_H__

class AssignNode : public Node {
public:
  AssignNode(const int & row, const int & col) : Node(row, col) {}
  AssignNode(const int & row, const int & col, const shared_ptr<Node> &lhs, const shared_ptr<Node> &rhs)
      : Node(row, col), lhs_(lhs), rhs_(rhs) {
  }
  ~AssignNode() {}
  void setLHS(const shared_ptr<Node> &lhs) { lhs_ = lhs; }
  void setRHS(const shared_ptr<Node> &rhs) { rhs_ = rhs; }
  shared_ptr<Node> getLHS() { return lhs_; }
  shared_ptr<Node> getRHS() { return rhs_; }
  string getHead() { return head_; }

private:
  string head_ = "Assign";
  shared_ptr<Node> lhs_;
  shared_ptr<Node> rhs_;
};

#endif /* __ASSIGN_H__ */
