
#ifndef __ASSIGN_H__
#define __ASSIGN_H__

class AssignNode : public Node, public NodeAcceptor<AssignNode> {
public:
  AssignNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw) : Node(row, col, endrow, endcol, raw) {}
  AssignNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw, const shared_ptr<Node> &lhs,
             const shared_ptr<Node> &rhs)
      : Node(row, col, endrow, endcol, raw), lhs_(lhs), rhs_(rhs) {}
  ~AssignNode() {}
  void setLHS(const shared_ptr<Node> &lhs) {
    lhs_ = lhs;
    lhs_->setParent(this);
  }
  void setRHS(const shared_ptr<Node> &rhs) {
    rhs_ = rhs;
    rhs_->setParent(this);
  }
  shared_ptr<Node> getLHS() const { return lhs_; }
  shared_ptr<Node> getRHS() const { return rhs_; }
  string getHead() const override { return head_; }
  void toCCode_(ostringstream &o) override {
    lhs_->toCCode_(o);
    o << " = ";
    rhs_->toCCode_(o);
    return;
  }
  void toString_(ostringstream &o) override {
    lhs_->toString_(o);
    o << " = ";
    rhs_->toString_(o);
    return;
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "AssignmentExpression";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["operator"] = "=";
    obj["left"] = lhs_->toEsprima_();
    obj["right"] = rhs_->toEsprima_();
    return Json(obj);
  }
  Json toJSON_() { return toEsprima_(); }

  bool hasChildren() const override {
    return lhs_ != nullptr || rhs_ != nullptr;
  }
  vector<shared_ptr<Node>> getChildren() override {
    if (hasChildren() == false) {
      return vector<shared_ptr<Node>>{};
    } else if (lhs_ != nullptr && rhs_ != nullptr) {
      return vector<shared_ptr<Node>>{lhs_, rhs_};
    } else if (lhs_ != nullptr) {
      return vector<shared_ptr<Node>>{lhs_};
    } else if (rhs_ != nullptr) {
      return vector<shared_ptr<Node>>{rhs_};
    } else {
      assert(false);
      return vector<shared_ptr<Node>>{};
    }
  }

private:
  string head_ = "Assign";
  shared_ptr<Node> lhs_;
  shared_ptr<Node> rhs_;
};

#endif /* __ASSIGN_H__ */
