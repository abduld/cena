
#ifndef __PARENT_H__
#define __PARENT_H__

class ParenNode : public Node, public NodeAcceptor<ParenNode> {
public:
  ParenNode(const int &row, const int &col) : Node(row, col) {}
  ParenNode(const int &row, const int &col, const shared_ptr<Node> &body)
      : Node(row, col), body_(body) {}
  ~ParenNode() {}
  void setBody(const shared_ptr<Node> &body) {
    body_ = body;
    body_->setParent(this);
  }
  shared_ptr<Node> getBody() const { return body_; }
  string getHead() const override { return head_; }
  void toCCode_(ostringstream &o) override {
    o << "(";
    body_->toCCode_(o);
    o << ")";
    return;
  }
  void toString_(ostringstream &o) override {
    o << "(";
    body_->toString_(o);
    o << ")";
    return;
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "ExpressionStatement";
    obj["line"] = row_;
    obj["column"] = col_;
    obj["expression"] = body_->toEsprima_();
    return Json(obj);
  }
  Json toJSON_() { return toEsprima_(); }

  bool hasChildren() const override {
    return body_ != nullptr;
  }
  vector<shared_ptr<Node> > getChildren() override {
    if (hasChildren() == false) {
      return vector<shared_ptr<Node> >{};
    } else {
      return vector<shared_ptr<Node> >{ body_};
    }
  }

private:
  string head_ = "Parenthesis";
  shared_ptr<Node> body_ = nullptr;
};

#endif /* __PARENT_H__ */
