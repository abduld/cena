
#ifndef __ASSIGN_H__
#define __ASSIGN_H__

class AssignNode : public Node {
public:
  AssignNode(const int &row, const int &col);
  AssignNode(const int &row, const int &col, const shared_ptr<Node> &lhs,
             const shared_ptr<Node> &rhs);
  ~AssignNode();
  void setLHS(const shared_ptr<Node> &lhs);
  void setRHS(const shared_ptr<Node> &rhs);
  shared_ptr<Node> getLHS() const;
  shared_ptr<Node> getRHS() const;
  string getHead() const override;
  virtual void toCCode_(ostringstream &o) override;
  virtual void toString_(ostringstream &o) override;
  virtual Json toJSON_() override;
  virtual Json toEsprima_() override;

private:
  static string head_;
  shared_ptr<Node> lhs_;
  shared_ptr<Node> rhs_;
};

#endif /* __ASSIGN_H__ */
