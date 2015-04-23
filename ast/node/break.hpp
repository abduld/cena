
#ifndef __BREAK_H__
#define __BREAK_H__

class BreakNode : public Node {
public:
  BreakNode(const int &row, const int &col, const int &endrow,
            const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~BreakNode() {}
  string getHead() const override { return head_; }

  bool isStatement() const override { return true; }

  void toCCode_(ostringstream &o) override { o << "break"; }
  void toString_(ostringstream &o) override { o << "break"; }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "BreakStmt";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"break\"}"; }
  void traverse(ASTVisitor *visitor) override {}

  bool hasChildren() const override { return false; }
  vector<shared_ptr<Node>> getChildren() override {
    return vector<shared_ptr<Node>>{};
  }

private:
  string head_ = "Break";
};

#endif /* __BREAK_H__ */
