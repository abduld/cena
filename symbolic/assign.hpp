
#ifndef __ASSIGN_H__
#define __ASSIGN_H__

class Assign : public Compound {
public:
  Assign() : Compound() {}
  Assign(Node *lhs, Node *rhs) : Compound() {
    push_back(lhs);
    push_back(rhs);
  }
  Assign(const shared_ptr<Node> &lhs, const shared_ptr<Node> &rhs)
      : Compound() {
    push_back(lhs);
    push_back(rhs);
  }
  ~Assign() {}
  void setLHS(const shared_ptr<Node> &lhs) { setPart(0, lhs); }

  void setRHS(const shared_ptr<Node> &rhs) { setPart(1, rhs); }
  shared_ptr<Node> getLHS() { return getPart(0); }
  shared_ptr<Node> setRHS() { return getPart(1); }
  string getHead() { return head_; }

private:
  string head_ = "Assign";
};

#endif /* __ASSIGN_H__ */
