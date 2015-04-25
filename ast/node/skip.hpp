

#ifndef __SKIP_H__
#define __SKIP_H__

class SkipStmtNode : public Node, public NodeAcceptor<SkipStmtNode> {
public:
  SkipStmtNode(const int &row, const int &col, const int &endrow,
               const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {
        isSkip(true);
      }
  string getHead() const override { return head_; }

  void toCCode_(ostringstream &o) override {}
  void toString_(ostringstream &o) override {}
  Json toEsprima_() override {
    Json::object obj{};
    return obj;
  }
  void toJSON_(ostringstream &o) {}

  bool hasChildren() const override { return false; }
  vector<shared_ptr<Node>> getChildren() override {
    return vector<shared_ptr<Node>>{};
  }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

private:
  string head_ = "Skip";
};
#endif /* __SKIP_H__ */
