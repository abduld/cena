

#ifndef __ATOM_H__
#define __ATOM_H__

template <typename T> class AtomNode : public Node {
public:
  AtomNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw)
      : Node(row, col, endrow, endcol, raw), init_(true) {}
  AtomNode(const int &row, const int &col, const int &endrow, const int &endcol,
           const string &raw, const T &v)
      : Node(row, col, endrow, endcol, raw), init_(true) {
    val_ = v;
  }
  vector<shared_ptr<Node>> getValues() {
    vector<shared_ptr<Node>> vec;
    shared_ptr<Node> v(this);
    vec.push_back(v);
    return vec;
  }
  static bool isAtomNode() { return true; }
  void setConstant(const T &val) { val_ = val; }
  T getConstant() const { return val_; }
  virtual void toCCode_(ostringstream &o) {
    // std::cout << getHead() << "   ::  " << getConstant() << std::endl;
    o << getConstant();
  }
  virtual void toString_(ostringstream &o) {
    if (init_) {
      toCCode_(o);
    } else {
      o << "uninitialized...";
    }
  }
  virtual void toJSON_(ostringstream &o) { toCCode_(o); }
  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = Json("Literal");
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = toString();
    return obj;
  }
  bool hasChildren() const override { return false; }
  vector<shared_ptr<Node>> getChildren() override {
    return vector<shared_ptr<Node>>{};
  }

private:
  bool init_ = false;
  T val_{};
};

class BooleanNode : public AtomNode<bool>, public NodeAcceptor<BooleanNode> {
public:
  BooleanNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<bool>(row, col, endrow, endcol, raw) {}
  BooleanNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const bool &v)
      : AtomNode<bool>(row, col, endrow, endcol, raw, v) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) { o << (getConstant() ? "true" : "false"); }

private:
  string head_ = "Boolean";
};

class SymbolNode : public AtomNode<string>, public NodeAcceptor<SymbolNode> {
public:
  SymbolNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw)
      : AtomNode<string>(row, col, endrow, endcol, raw) {}
  SymbolNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const string &v)
      : AtomNode<string>(row, col, endrow, endcol, raw, v) {}
  SymbolNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const char *v)
      : AtomNode<string>(row, col, endrow, endcol, raw, string(v)) {}
  string getHead() const { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

private:
  string head_ = "Symbol";
};

class StringNode : public AtomNode<string>, public NodeAcceptor<StringNode> {
public:
  StringNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw)
      : AtomNode<string>(row, col, endrow, endcol, raw) {}
  StringNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const string &v)
      : AtomNode<string>(row, col, endrow, endcol, raw, v) {}
  StringNode(const int &row, const int &col, const int &endrow,
             const int &endcol, const string &raw, const char *v)
      : AtomNode<string>(row, col, endrow, endcol, raw, string(v)) {}
  string getHead() const { return head_; }

  void toCCode_(ostringstream &o) { o << "\"" << getConstant() << "\""; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

private:
  string head_ = "String";
};
#endif /* __ATOM_H__ */
