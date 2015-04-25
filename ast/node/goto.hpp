
#ifndef __GOTO_H__
#define __GOTO_H__

class GotoNode : public Node {
public:
  GotoNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw)
      : Node(row, col, endrow, endcol, raw) {isStatement(true);}
  ~GotoNode() {}
  string getHead() const override { return head_; }

  void setLabel(const shared_ptr<Node> &nd) {
    lbl_ = nd;
    lbl_->setParent(this);
  }
  shared_ptr<Node> getLabel() const { return lbl_; }

  void toCCode_(ostringstream &o) override {
    assert(lbl_ != nullptr);
    o << "goto ";
    if (lbl_ != nullptr) {
      lbl_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {
    assert(lbl_ != nullptr);
    o << "goto ";
    if (lbl_ != nullptr) {
      lbl_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "Goto";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (lbl_ != nullptr) {
      obj["target"] = lbl_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"goto\"}"; }
  void traverse(ASTVisitor *visitor) override {
    if (lbl_ != nullptr) {
      lbl_->traverse(visitor);
    }
  }
  bool hasChildren() const override { return lbl_ != nullptr; }
  vector<shared_ptr<Node>> getChildren() override {
    if (!hasChildren()) {
      return vector<shared_ptr<Node>>{};
    } else {
      return vector<shared_ptr<Node>>{lbl_};
    }
  }

private:
  string head_ = "Goto";
  shared_ptr<Node> lbl_ = nullptr;
};

#endif /* __GOTO_H__ */
