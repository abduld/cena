

#ifndef __ATOM_H__
#define __ATOM_H__

template <typename T> class AtomNode : public Node {
public:
  AtomNode(const int &row, const int &col) : Node(row, col), init_(true) {}
  AtomNode(const int &row, const int &col, const T &v)
      : Node(row, col), init_(true) {
    val_ = v;
  }
  vector<shared_ptr<Node> > getValues() {
    vector<shared_ptr<Node> > vec;
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
    ostringstream o;
    obj["type"] = Json("literal");
    obj["line"] = row_;
    obj["column"] = col_;
    o << val_;
    obj["value"] = o.str();
    return obj;
  }
  bool hasChildren() const override { return false; }
  vector<shared_ptr<Node> > getChildren() override {
    return vector<shared_ptr<Node> >{};
  }

private:
  bool init_ = false;
  T val_{};
};

class BooleanNode : public AtomNode<bool>, public NodeAcceptor<BooleanNode> {
public:
  BooleanNode(const int &row, const int &col) : AtomNode<bool>(row, col) {}
  BooleanNode(const int &row, const int &col, const bool &v)
      : AtomNode<bool>(row, col, v) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) { o << (getConstant() ? "true" : "false"); }

private:
  string head_ = "Boolean";
};

class SymbolNode : public AtomNode<string>, public NodeAcceptor<SymbolNode> {
public:
  SymbolNode(const int &row, const int &col) : AtomNode<string>(row, col) {}
  SymbolNode(const int &row, const int &col, const string &v)
      : AtomNode<string>(row, col, v) {}
  SymbolNode(const int &row, const int &col, const char *v)
      : AtomNode<string>(row, col, string(v)) {}
  string getHead() const { return head_; }
  void traverse(ASTVisitor * visitor) override {
	  accept(visitor);
  }

private:
  string head_ = "Symbol";
};

class StringNode : public AtomNode<string>, public NodeAcceptor<StringNode> {
public:
  StringNode(const int &row, const int &col) : AtomNode<string>(row, col) {}
  StringNode(const int &row, const int &col, const string &v)
      : AtomNode<string>(row, col, v) {}
  StringNode(const int &row, const int &col, const char *v)
      : AtomNode<string>(row, col, string(v)) {}
  string getHead() const { return head_; }

  void toCCode_(ostringstream &o) { o << "\"" << getConstant() << "\""; }
  void traverse(ASTVisitor * visitor) override {
	  accept(visitor);
  }


private:
  string head_ = "String";
};
#endif /* __ATOM_H__ */
