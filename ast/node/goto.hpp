
#ifndef __GOTO_H__
#define __GOTO_H__

class GotoNode : public Node {
public:
  GotoNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw, const shared_ptr<Node> &label)
      : Node(row, col, endrow, endcol, raw), label_(label) {}
  ~GotoNode() {}
  string getHead() { return head_; }

  virtual bool isStatement() const override { return true; }
  void toCCode_(ostringstream &o) override {
    assert(label_ != nullptr);
    o << "goto ";
    if (label_ != nullptr) {
      label_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {
    assert(label_ != nullptr);
    o << "goto ";
    if (label_ != nullptr) {
      label_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "Goto";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (label_ != nullptr) {
      obj["target"] = label_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"goto\"}"; }
  void traverse(ASTVisitor *visitor) override {
    if (label_ != nullptr) {
      label_->traverse(visitor);
    }
  }
private:
  string head_ = "Goto";
  shared_ptr<Node> label_ = nullptr;
};

#endif /* __GOTO_H__ */
