
#ifndef __CASE_H__
#define __CASE_H__

class CaseNode : public Node {
public:
  CaseNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~CaseNode() {}
  string getHead() { return head_; }


  virtual bool isStatement() const override { return true; }
  void toCCode_(ostringstream &o) override {
    assert(lhs_ != nullptr);
    assert(body_ != nullptr);
    o << "case ";
    if (lhs_ != nullptr) {
      lhs_->toCCode_(o);
    }
    if (rhs_ != nullptr) {
      rhs_->toCCode_(o);
    }
    if (body_ != nullptr) {
      body_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {
    assert(lhs_ != nullptr);
    assert(body_ != nullptr);
    o << "case ";
    if (lhs_ != nullptr) {
      lhs_->toString_(o);
    }
    if (rhs_ != nullptr) {
      rhs_->toString_(o);
    }
    if (body_ != nullptr) {
      body_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "CaseStatement";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (lhs_ != nullptr) {
      obj["lhs"] = lhs_->toEsprima_();
    }
    if (rhs_ != nullptr) {
      obj["rhs"] = rhs_->toEsprima_();
    }
    if (body_ != nullptr) {
      obj["body"] = body_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"case\"}"; }
  void traverse(ASTVisitor *visitor) override {
    if (lhs_ != nullptr) {
      lhs_->traverse(visitor);
    }
    if (rhs_ != nullptr) {
      rhs_->traverse(visitor);
    }
    if (body_ != nullptr) {
      body_->traverse(visitor);
    }
  }
private:
  string head_ = "Case";
  shared_ptr<Node> lhs_ = nullptr;
  shared_ptr<Node> rhs_ = nullptr;
  shared_ptr<Node> body_ = nullptr;
};

#endif /* __CASE_H__ */
