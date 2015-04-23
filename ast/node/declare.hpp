
#ifndef __DECLARE_H__
#define __DECLARE_H__

class DeclareNode : public Node {
public:
  DeclareNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  void setIdentifier(const shared_ptr<IdentifierNode> &id) { id_ = id; }
  void setType(const shared_ptr<TypeNode> &typ) { typ_ = typ; }
  void setInitializer(const shared_ptr<Node> &init) { init_ = init; }
  shared_ptr<Node> getIdentifier() const { return id_; }
  shared_ptr<Node> getType() const { return typ_; }
  shared_ptr<Node> getInitializer() const { return init_; }
  bool hasInitializer() const { return init_ != nullptr; }
  string getHead() const override { return head_; }

  void toCCode_(ostringstream &o) override {
    assert(typ_ != nullptr);
    assert(id_ != nullptr);
    typ_->toCCode_(o);
    o << " ";
    id_->toCCode_(o);
    if (hasInitializer()) {
      o << " = ";
      init_->toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {

    assert(typ_ != nullptr);
    assert(id_ != nullptr);
    typ_->toString_(o);
    o << " ";
    id_->toString_(o);
    if (hasInitializer()) {
      o << " = ";
      init_->toString_(o);
    }
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  Json toEsprima_() override {
    Json::object obj;
    Json::object decl;
    decl["type"] = "VariableDeclarator";
    decl["loc"] = getLocation();
    decl["raw"] = raw_;
    decl["cform"] = toCCode();
    decl["kind"] = typ_->toEsprima_();
    decl["id"] = id_->toEsprima_();
    if (hasInitializer()) {
      decl["init"] = init_->toEsprima_();
    }
    obj["type"] = "VariableDeclaration";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["declarations"] = vector<Json>{decl};
    return obj;
  }
  void traverse(ASTVisitor *visitor) override {
    id_->traverse(visitor);
    typ_->traverse(visitor);
    if (hasInitializer()) {
      init_->traverse(visitor);
    }
  }

private:
  string head_ = "Declare";
  shared_ptr<IdentifierNode> id_ = nullptr;
  shared_ptr<TypeNode> typ_ = nullptr;
  shared_ptr<Node> init_ = nullptr;
};

#endif /* __IDENTIFIER_H__ */
