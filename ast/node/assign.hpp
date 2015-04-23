
#ifndef __ASSIGN_H__
#define __ASSIGN_H__

class AssignOp : public StringNode {
public:
  AssignOp(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw, const char *s)
      : StringNode(row, col, endrow, endcol, raw, s) {}
  AssignOp(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw, const string &s)
      : StringNode(row, col, endrow, endcol, raw, s) {}
  string getHead() const override { return head_; }
  void toCCode_(ostringstream &o) override { o << getConstant(); }
  void toString_(ostringstream &o) override { toCCode_(o); }

private:
  string head_ = "BinaryOp";
};

class AssignNode : public Node, public NodeAcceptor<AssignNode> {
public:
  AssignNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  AssignNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const shared_ptr<Node> &lhs,
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
    o << " ";
    op_->toCCode_(o);
    o << " ";
    rhs_->toCCode_(o);
    return;
  }
  void toString_(ostringstream &o) override {
    lhs_->toString_(o);
    o << " ";
    op_->toString_(o);
    o << " ";
    rhs_->toString_(o);
    return;
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "AssignmentExpression";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["operator"] = op_->toString();
    obj["left"] = lhs_->toEsprima_();
    obj["right"] = rhs_->toEsprima_();
    return Json(obj);
  }
  Json toJSON_() override { return toEsprima_(); }

  void setOperator(const string &op) {
    op_ = shared_ptr<AssignOp>(
        new AssignOp(row_, col_, endrow_, endcol_, op, op));
  }
  void setOperator(const shared_ptr<AssignOp> &op) { op_ = op; }
  shared_ptr<Node> getOperator() const { return op_; }

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
  shared_ptr<AssignOp> op_ = nullptr;
};

#endif /* __ASSIGN_H__ */
