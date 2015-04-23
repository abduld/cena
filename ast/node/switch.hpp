

#ifndef __SWITCH_H__
#define __SWITCH_H__

class SwitchNode : public Node {
public:
  SwitchNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  string getHead() const override { return head_; }
  void setCondition(const shared_ptr<Node> &cond) {
    cond_ = cond;
    cond_->setParent(this);
  }
  shared_ptr<Node> getCondition() { return cond_; }
  void setBody(const shared_ptr<Node> &nd) {
    body_ = nd;
    body_->setParent(this);
  }
  shared_ptr<Node> getBody() { return body_; }
  void toCCode_(ostringstream &o) override {
    assert(cond_ != nullptr);
    assert(body_ != nullptr);
    o << "switch (";
    cond_->toCCode_(o);
    o << ") {\n";
    body_->toCCode_(o);
    o << "} \n";
  }
  void toString_(ostringstream &o) override {
    assert(cond_ != nullptr);
    assert(body_ != nullptr);
    o << "switch (";
    cond_->toString_(o);
    o << ") {\n";
    body_->toString_(o);
    o << "\n}";
  }
  bool isBlock() const override { return true; }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"switch\"}"; }
  Json toEsprima_() override {
    Json::object obj;
    vector<Json> args;
    obj["type"] = "SwitchStatement";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["test"] = cond_->toEsprima_();
    if (body_ != nullptr) {
      obj["body"] = body_->toEsprima_();
    }
    return obj;
  }
  bool hasBody() const { return body_ != nullptr; }
  void traverse(ASTVisitor *visitor) override {
    cond_->traverse(visitor);
    body_->traverse(visitor);
  }
  bool hasChildren() const override {
    return cond_ != nullptr && body_ != nullptr;
  }
  vector<shared_ptr<Node>> getChildren() override {
    if (!hasChildren()) {
      return vector<shared_ptr<Node>>{};
    } else {
      vector<shared_ptr<Node>> children{};
      if (cond_ != nullptr) {
        children.push_back(cond_);
      }
      if (body_ != nullptr) {
        children.push_back(body_);
      }
      return children;
    }
  }

private:
  string head_ = "Switch";
  shared_ptr<Node> cond_ = nullptr;
  shared_ptr<Node> body_ = nullptr;
};
#endif /* __SWITCH_H__ */
