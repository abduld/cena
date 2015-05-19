

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
  string getHead() const override { return head_; }

  void setCondition(const shared_ptr<Node> &cond) {
    cond_ = cond;
    cond_->setParent(this);
  }
  void setBody(const shared_ptr<Node> &body) {
    body_ = body;
    body_->setParent(this);
  }

  void toCCode_(ostringstream &o) override {
    assert(cond_ != nullptr);
    assert(body_ != nullptr);
    o << "while (";
    cond_->toCCode_(o);
    o << ")\n";
    body_->toCCode_(o);
  }
  void toString_(ostringstream &o) override {

    assert(cond_ != nullptr);
    assert(body_ != nullptr);
    o << "while (";
    cond_->toString_(o);
    o << ")\n";
    body_->toString_(o);
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  Json toEsprima_() override {
    Json::object obj;
    vector<Json> args;
    obj["type"] = "WhileStatement";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["test"] = cond_->toEsprima_();
    obj["body"] = body_->toEsprima_();
    return obj;
  }
  bool hasChildren() const override {
    return cond_ != nullptr || body_ != nullptr;
  }
  vector<shared_ptr<Node>> getChildren() override {
    if (hasChildren() == false) {
      return vector<shared_ptr<Node>>{};
    } else if (cond_ != nullptr && body_ == nullptr) {
      return vector<shared_ptr<Node>>{cond_};
    } else if (cond_ == nullptr && body_ != nullptr) {
      return vector<shared_ptr<Node>>{body_};
    } else  {
      return vector<shared_ptr<Node>>{cond_, body_};
    }
  }
  void traverse(ASTVisitor *visitor) override {
    if (cond_) {
      cond_->traverse(visitor);
    }
    if (body_) {
      body_->traverse(visitor);
    }
  }
private:
  string head_ = "While";
  shared_ptr<Node> cond_ = nullptr;
  shared_ptr<Node> body_ = nullptr;
};
#endif /* __WHILE_H__ */
