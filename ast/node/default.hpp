
#ifndef __DEFAULT_H__
#define __DEFAULT_H__

class DefaultNode : public Node {
public:
  DefaultNode(const int &row, const int &col, const int &endrow, const int &endcol,
          const string &raw)
      : Node(row, col, endrow, endcol, raw) {}
  ~DefaultNode() {}

  virtual bool isStatement() const override { return true; }

  string getHead() const override { return head_; }
  void toCCode_(ostringstream &o) override {
    o << "default";
  }
  void toString_(ostringstream &o) override {
    o << "default";
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "DefaultStmt";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    return obj;
  }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"default\"}"; }
  void traverse(ASTVisitor *visitor) override {
  }
private:
  string head_ = "Default";
};

#endif /* __DEFAULT_H__ */
