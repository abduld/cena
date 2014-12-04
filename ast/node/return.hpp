
#ifndef __RETURN_H__
#define __RETURN_H__

class ReturnNode : public Node {
public:
  ReturnNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ReturnNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const shared_ptr<Node> &nd)
      : Node(row, col, endrow, endcol, raw) {
    setReturnValue(nd);
  }
  ~ReturnNode() {}
  void setReturnValue(const shared_ptr<Node> &arg) { ret_ = arg; }
  shared_ptr<Node> getReturnValue() const { return ret_; }
  string getHead() const { return head_; }
  bool isStatement() const { return true; }
  void toCCode_(ostringstream &o) {
    o << "return";
    if (ret_ != nullptr) {
      o << " ";
      ret_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) {
    o << "return";
    if (ret_ != nullptr) {
      o << " ";
      ret_->toString_(o);
    }
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "ReturnStatement";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (ret_ != nullptr) {
      obj["argument"] = ret_->toEsprima_();
    }
    return obj;
  }
  void traverse(ASTVisitor *visitor) override {
    if (ret_ != nullptr) {
      ret_->traverse(visitor);
    }
  }

private:
  string head_ = "Return";
  shared_ptr<Node> ret_ = nullptr;
};

#endif /* __RETURN_H__ */
