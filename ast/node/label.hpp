
#ifndef __LABEL_H__
#define __LABEL_H__

class LabelStmtNode : public Node {
public:
  LabelStmtNode(const int &row, const int &col, const int &endrow,
                const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~LabelStmtNode() {}
  string getHead() const override { return head_; }

  void setBody(const shared_ptr<Node> &nd) {
    if (body_ == nullptr) {
      body_ = shared_ptr<CompoundNode>(
          new CompoundNode(row_, col_, endrow_, endcol_, raw_));
      body_->setParent(this);
      addChild(body_);
    }
    *body_ <<= nd;
    body_->setParent(this);
  }
  shared_ptr<Node> getBody() const { return body_; }
  void setLabel(const shared_ptr<Node> &nd) {
    lbl_ = nd;
    lbl_->setParent(this);
  }
  shared_ptr<Node> getLabel() const { return lbl_; }

  bool isStatement() const override { return true; }

  void toCCode_(ostringstream &o) override {
    assert(lbl_ != nullptr);
    assert(body_ != nullptr);
    if (lbl_ != nullptr) {
      lbl_->toCCode_(o);
    }
    o << ":\n";
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
    o << ":\n";
    if (body_ != nullptr) {
      body_->toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "LabelStmt";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    if (lbl_ != nullptr) {
      obj["label"] = lbl_->toEsprima_();
    }
    if (body_ != nullptr) {
      obj["body"] = body_->toEsprima_();
    }
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"label-stmt\"}"; }
  void traverse(ASTVisitor *visitor) override {
    if (lbl_ != nullptr) {
      lbl_->traverse(visitor);
    }
    if (body_ != nullptr) {
      body_->traverse(visitor);
    }
  }
  bool hasChildren() const override {
    return lbl_ != nullptr && body_ != nullptr;
  }
  vector<shared_ptr<Node>> getChildren() override {
    if (!hasChildren()) {
      return vector<shared_ptr<Node>>{};
    } else {
      vector<shared_ptr<Node>> children{};
      if (lbl_ != nullptr) {
        children.push_back(lbl_);
      }
      if (body_ != nullptr) {
        children.push_back(body_);
      }
      return children;
    }
  }

private:
  string head_ = "LabelStmt";
  shared_ptr<Node> lbl_ = nullptr;
  shared_ptr<CompoundNode> body_ = nullptr;
};

class LabelDeclNode : public Node {
public:
  LabelDeclNode(const int &row, const int &col, const int &endrow,
                const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~LabelDeclNode() {}
  string getHead() const override { return head_; }

  void setName(const string &name) {
    init_ = true;
    name_ = name;
  }
  string getName() const { return name_; }

  void toCCode_(ostringstream &o) override {
    assert(init_);
    o << name_;
  }
  void toString_(ostringstream &o) override {
    assert(init_);
    o << name_;
    o << ": ";
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "LabelDecl";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["name"] = name_;
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"label-decl\"}"; }
  void traverse(ASTVisitor *visitor) override {}
  bool hasChildren() const override { return false; }
  vector<shared_ptr<Node>> getChildren() override {
    return vector<shared_ptr<Node>>{};
  }

private:
  string head_ = "LabelDecl";
  bool init_ = false;
  string name_{};
};

#endif /* __LABEL_H__ */
