

#ifndef __CONDITIONAL_H__
#define __CONDITIONAL_H__

class ConditionalNode : public Node {
public:
  ConditionalNode(const int &row, const int &col, const int &endrow,
                  const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ConditionalNode(const int &row, const int &col, const int &endrow,
                  const int &endcol, const string &raw,
                  const shared_ptr<Node> &cond,
                  const shared_ptr<Node> &thenPart)
      : Node(row, col, endrow, endcol, raw), cond_(cond), then_(thenPart) {
    cond_->setParent(this);
    then_->setParent(this);
  }
  ConditionalNode(const int &row, const int &col, const int &endrow,
                  const int &endcol, const string &raw,
                  const shared_ptr<Node> &cond,
                  const shared_ptr<Node> &thenPart,
                  const shared_ptr<Node> &elsePart)
      : Node(row, col, endrow, endcol, raw), cond_(cond), then_(thenPart),
        else_(elsePart) {
    cond_->setParent(this);
    then_->setParent(this);
    else_->setParent(this);
  }
  string getHead() const override { return head_; }
  void setCondition(const shared_ptr<Node> &cond) {
    cond_ = cond;
    cond_->setParent(this);
  }
  void setThen(const shared_ptr<Node> &nd) {
    then_ = nd;
    then_->setParent(this);
  }
  void setElse(const shared_ptr<Node> &nd) {
    else_ = nd;
    else_->setParent(this);
  }
  void toCCode_(ostringstream &o) {
    assert(cond_ != nullptr);
    assert(then_ != nullptr);
    ;
    cond_->toCCode_(o);
    o << " ? ";
    then_->toCCode_(o);
    o << " : ";
    else_->toCCode_(o);
  }
  void toString_(ostringstream &o) {

    assert(cond_ != nullptr);
    assert(then_ != nullptr);
    cond_->toString_(o);
    o << " ? ";
    then_->toString_(o);
    o << " : ";
    else_->toString_(o);
  }
  bool isBlock() const { return true; }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  Json toEsprima_() override {
    Json::object obj;
    vector<Json> args;
    obj["type"] = "ConditionalExpression";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["test"] = cond_->toEsprima_();
    obj["consequent"] = then_->toEsprima_();
    obj["alternate"] = else_->toEsprima_();
    return obj;
  }
  bool hasChildren() const override {
    return cond_ != nullptr || then_ != nullptr || else_ != nullptr;
  }
  vector<shared_ptr<Node>> getChildren() override {
    if (hasChildren() == false) {
      return vector<shared_ptr<Node>>{};
    } else if (cond_ != nullptr && then_ != nullptr && else_ != nullptr) {
      return vector<shared_ptr<Node>>{cond_, then_, else_};
    } else if (cond_ != nullptr && then_ != nullptr) {
      return vector<shared_ptr<Node>>{cond_, then_};
    } else if (cond_ != nullptr && else_ != nullptr) {
      return vector<shared_ptr<Node>>{cond_, else_};
    } else {
      assert(then_ != nullptr && else_ != nullptr && cond_ == nullptr);
      return vector<shared_ptr<Node>>{then_, else_};
    }
  }
  void traverse(ASTVisitor *visitor) override {
    cond_->traverse(visitor);
    then_->traverse(visitor);
    if (else_) {
      else_->traverse(visitor);
    }
  }

private:
  string head_ = "Conditional";
  shared_ptr<Node> cond_ = nullptr;
  shared_ptr<Node> then_ = nullptr;
  shared_ptr<Node> else_ = nullptr;
};
#endif /* __CONDITIONAL_H__ */
