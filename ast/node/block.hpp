
#ifndef __BLOCK_H__
#define __BLOCK_H__

class BlockNode : public CompoundNode {
public:
  BlockNode(const int &row, const int &col, const int &endrow,
            const int &endcol, const string &raw)
      : CompoundNode(row, col, endrow, endcol, raw) {
    isBlock(true);
  }
  BlockNode(const int &row, const int &col, const int &endrow,
            const int &endcol, const string &raw, const shared_ptr<Node> &nd)
      : CompoundNode(row, col, endrow, endcol, raw) {
    isBlock(true);
    push_back(nd);
  }
  ~BlockNode() {}
  string getHead() const override { return head_; }
  Json toEsprima_() override {
    Json::object obj;
    vector<Json> body;
    obj["type"] = "BlockStatement";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    for (auto stmt : getValues()) {
      body.push_back(stmt->toEsprima_());
    }
    obj["body"] = body;
    return obj;
  }

private:
  string head_ = "Block";
};

#endif /* __BLOCK_H__ */
