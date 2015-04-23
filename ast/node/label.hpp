
#ifndef __LABEL_H__
#define __LABEL_H__

class LabelNode : public Node {
public:
  LabelNode(const int &row, const int &col, const int &endrow, const int &endcol,
        const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~LabelNode() {}
  string getHead() { return head_; }

  void setBody(const shared_ptr<Node> & nd) { body_ = nd; }
  shared_ptr<Node> getBody(const shared_ptr<Node> & nd) const { return body_; }
  void setLabel(const shared_ptr<Node> & nd) { lbl_ = nd; }
  shared_ptr<Node> getLabel(const shared_ptr<Node> & nd) const { return lbl_; }

  virtual bool isStatement() const override { return true; }

  void toCCode_(ostringstream &o) override {
    assert(lbl_ != nullptr);
    assert(body_ != nullptr);
    if (lbl_ != nullptr) {
      lbl_->toCCode_(o);
    }
    o << ": ";
    if (body_ != nullptr) {
      body_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {
    assert(lbl_ != nullptr);
    assert(body_ != nullptr);
    if (lbl_ != nullptr) {
      lbl_->toString_(o);
    }
    o << ": ";
    if (body_ != nullptr) {
      body_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "DefaultStmt";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (lbl_ != nullptr) {
      obj["label"] =  lbl_->toEsprima_();
    }
    if (body_ != nullptr) {
      obj["body"] =  body_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"label\"}"; }
  void traverse(ASTVisitor *visitor) override {
    if (lbl_ != nullptr) {
      lbl_->traverse(visitor);
    }
    if (body_ != nullptr) {
      body_->traverse(visitor);
    }
  }
private:
  string head_ = "Label";
  shared_ptr<Node> lbl_ = nullptr;
  shared_ptr<Node> body_ = nullptr;
};

#endif /* __LABEL_H__ */
