

#ifndef __FOR_H__
#define __FOR_H__

class ForNode : public Node, public NodeAcceptor<ForNode> {
public:
  ForNode(const int &row, const int &col, const int &endrow, const int &endcol,
          const string &raw)
      : Node(row, col, endrow, endcol, raw) {
        isCompound(true);

    isBlock(true);
      }
  string getHead() const override { return head_; }
  void setInit(const shared_ptr<Node> &nd) {
    init_ = nd;
    init_->isStatement(false);
    init_->setParent(this);
    addChild(init_);
  }
  void setCond(const shared_ptr<Node> &nd) {
    cond_ = nd;
    init_->isStatement(false);
    cond_->setParent(this);
    addChild(cond_);
  }
  void setInc(const shared_ptr<Node> &nd) {
    inc_ = nd;
    init_->isStatement(false);
    inc_->setParent(this);
    addChild(inc_);
  }
  void setBody(const shared_ptr<Node> &nd) {
    if (body_ == nullptr) {
      body_ = shared_ptr<BlockNode>(
          new BlockNode(row_, col_, endrow_, endcol_, raw_));
      body_->setParent(this);
      addChild(body_);
    }
    *body_ <<= nd;
  }
  void toCCode_(ostringstream &o) override {
    o << "for (";
    if (init_ != nullptr) {
      init_->toCCode_(o);
    }
    o << ";";
    if (cond_ != nullptr) {
      cond_->toCCode_(o);
    }
    o << ";";
    if (inc_ != nullptr) {
      inc_->toCCode_(o);
    }
    o << ") ";
    body_->toCCode_(o);
  }
  void toString_(ostringstream &o) override { toCCode_(o); }

  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "ForStatement";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["init"] = init_->toEsprima_();
    obj["test"] = cond_->toEsprima_();
    obj["update"] = inc_->toEsprima_();
    obj["body"] = body_->toEsprima_();
    return obj;
  }

  void traverse(ASTVisitor *visitor) override {
    init_->traverse(visitor);
    cond_->traverse(visitor);
    inc_->traverse(visitor);
    body_->traverse(visitor);
  }

private:
  string head_ = "For";
  shared_ptr<Node> init_ = nullptr;
  shared_ptr<Node> cond_ = nullptr;
  shared_ptr<Node> inc_ = nullptr;
  shared_ptr<BlockNode> body_ = nullptr;
};
#endif /* __FOR_H__ */
