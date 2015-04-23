
#ifndef __DEFAULT_H__
#define __DEFAULT_H__

class DefaultNode : public Node {
public:
  DefaultNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~DefaultNode() {}

  bool isStatement() const override { return true; }

  void setBody(const shared_ptr<Node> &nd) {
    body_ = nd;
    body_->setParent(this);
  }
  shared_ptr<Node> getBody() const { return body_; }

  string getHead() const override { return head_; }
  void toCCode_(ostringstream &o) override {
    o << "default";
    if (body_) {
      body_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {
    o << "default ";
    if (body_) {
      body_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "DefaultStmt";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (body_) {
      obj["body"] = body_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"default\"}"; }
  void traverse(ASTVisitor *visitor) override {
    if (body_) {
      body_->traverse(visitor);
    }
  }
  bool hasChildren() const override { return body_ != nullptr; }
  vector<shared_ptr<Node>> getChildren() override {
    if (!hasChildren()) {
      return vector<shared_ptr<Node>>{};
    } else {
      return vector<shared_ptr<Node>>{body_};
    }
  }

private:
  string head_ = "Default";
  shared_ptr<Node> body_ = nullptr;
};

#endif /* __DEFAULT_H__ */
