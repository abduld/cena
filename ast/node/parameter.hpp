
#ifndef __PARAMETER_H__
#define __PARAMETER_H__

class ParameterNode : public Node {
public:
  ParameterNode(const int &row, const int &col, const int &endrow,
                const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  void setIdentifier(const shared_ptr<IdentifierNode> &id) { id_ = id; }
  void setType(const shared_ptr<TypeNode> &typ) { typ_ = typ; }
  shared_ptr<Node> getIdentifier() const { return id_; }
  shared_ptr<Node> getType() const { return typ_; }
  string getHead() const override { return head_; }

  bool isArgument() const override { return true; }
  void toCCode_(ostringstream &o) override {
    assert(typ_ != nullptr);
    assert(id_ != nullptr);
    typ_->toCCode_(o);
    o << " ";
    id_->toCCode_(o);
  }
  void toString_(ostringstream &o) override {

    assert(typ_ != nullptr);
    assert(id_ != nullptr);
    typ_->toString_(o);
    o << " ";
    id_->toString_(o);
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "ParameterExpression";
    if (typ_ != nullptr) {
      obj["kind"] = typ_->toEsprima_();
    }
    obj["data"] = id_->toEsprima_();
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }

private:
  string head_ = "Parameter";
  shared_ptr<IdentifierNode> id_ = nullptr;
  shared_ptr<TypeNode> typ_ = nullptr;
};

#endif /* __IDENTIFIER_H__ */
