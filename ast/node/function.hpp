
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

class FunctionNode : public Node {
public:
  FunctionNode(const int &row, const int &col, const int &endrow,
               const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw), params_(nullptr) {}
  void setReturnType(const shared_ptr<TypeNode> &typ) { ret_ = typ; }
  shared_ptr<TypeNode> getReturnType() const { return ret_; }
  void setName(const shared_ptr<IdentifierNode> &id) { name_ = id; }
  shared_ptr<IdentifierNode> getName() const { return name_; }
  void addParameter(const shared_ptr<Node> &nd) {
    if (params_ == nullptr) {
      params_ = shared_ptr<CompoundNode>(
          new CompoundNode(row_, col_, endrow_, endcol_, raw_));
    }
    *params_ <<= nd;
  }
  void addAttribute(const string &str) { attributes_.push_back(str); }
  void setBody(const shared_ptr<BlockNode> &blk) {
    body_ = blk;
  }
  shared_ptr<BlockNode> getBody() const { return body_; }
  shared_ptr<BlockNode> getOrInitBody() {
    if (body_ == nullptr) {
      body_ = shared_ptr<BlockNode>(
            new BlockNode(row_, col_, endrow_, endcol_, raw_));
    }
    return body_;
  }

  void toCCode_(ostringstream &o) override {
    assert(ret_ != nullptr);
    assert(name_ != nullptr);
    for (auto attr : attributes_) {
      o << attr;
      o << " ";
    }
    ret_->toCCode_(o);
    o << " ";
    name_->toCCode_(o);
    o << "(";
    if (params_ != nullptr) {
      params_->toCCode_(o);
    }
    o << ")";
    if (body_ != nullptr) {
      body_->toCCode_(o);
    } else {
      o << ";\n";
    }
  }
  void toString_(ostringstream &o) override {
    assert(ret_ != nullptr);
    assert(name_ != nullptr);
    for (auto attr : attributes_) {
      o << attr;
      o << " ";
    }
    ret_->toString_(o);
    o << " ";
    name_->toString_(o);
    o << "(";
    if (params_ != nullptr) {
      params_->toString_(o);
    }
    o << ")";
    if (body_ != nullptr) {
      body_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "Function";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (name_ != nullptr) {
	    obj["id"] = name_->getName();
    }
    obj["attributes"] = attributes_;
    if (params_ != nullptr) {
	    obj["params"] = params_->toEsprima_();
    }
    if (body_ != nullptr) {
	    obj["body"] = body_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  void traverse(ASTVisitor *visitor) override {
    name_->traverse(visitor);
    if (params_ != nullptr) {
      params_->traverse(visitor);
    }
    if (body_ != nullptr) {
	    body_->traverse(visitor);
    }
    if (qualifiers_ != nullptr) {
      qualifiers_->traverse(visitor);
    }
  }

private:
  shared_ptr<TypeNode> ret_ = nullptr;
  shared_ptr<IdentifierNode> name_ = nullptr;
  shared_ptr<CompoundNode> params_ = nullptr;
  shared_ptr<BlockNode> body_ = nullptr;
  shared_ptr<TypeNode> qualifiers_ = nullptr;
  vector<string> attributes_{};
};

#endif /* __FUNCTION_H__ */
