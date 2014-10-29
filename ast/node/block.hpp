
#ifndef __BLOCK_H__
#define __BLOCK_H__

class BlockNode : public CompoundNode {
public:
  BlockNode(const int &row, const int &col) : CompoundNode(row, col) {}
  BlockNode(const int &row, const int &col, const shared_ptr<Node> &nd)
      : CompoundNode(row, col) {
    vals_.push_back(nd);
  }
  ~BlockNode() {}
  string getHead() const { return head_; }
  bool isBlock() const { return true; }
  Json toEsprima_() override {
    Json::object obj;
    vector<Json> body;
    obj["type"] = "BlockStatement";
    obj["line"] = row;
    obj["column"] = column;
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
