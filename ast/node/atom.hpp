

#ifndef __ATOM_H__
#define __ATOM_H__

template <typename T> class AtomNode : public Node {
public:
  AtomNode(const int &row, const int &col) : Node(row, col), init_(true) {}
  AtomNode(const int &row, const int &col, const T &v)
      : Node(row, col), init_(true) {
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
  virtual Json toEsprima_() {
      Json::object obj;
      obj["type"] = Json("literal");
      obj["value"] = Json(val_);
      return obj;
  }

private:
  bool init_ = false;
  T val_{};
};

class BooleanNode : public AtomNode<bool> {
public:
  BooleanNode(const int &row, const int &col) : AtomNode<bool>(row, col) {}
  BooleanNode(const int &row, const int &col, const bool &v)
      : AtomNode<bool>(row, col, v) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) { o << (getConstant() ? "true" : "false"); }

private:
  string head_ = "Boolean";
};

class CharacterNode : public AtomNode<unsigned char> {
public:
  CharacterNode(const int &row, const int &col)
      : AtomNode<unsigned char>(row, col) {}
  CharacterNode(const int &row, const int &col, const unsigned char &v)
      : AtomNode<unsigned char>(row, col, v) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) {
    o << "'" << string(1, getConstant()) << "'";
  }

private:
  string head_ = "Character";
};
class IntegerNode : public virtual AtomNode<int64_t> {
public:
  IntegerNode(const int &row, const int &col) : AtomNode<int64_t>(row, col) {}
  IntegerNode(const int &row, const int &col, const int64_t &v)
      : AtomNode<int64_t>(row, col, v) {}
  string getHead() const { return head_; }

private:
  string head_ = "Integer";
};

class RealNode : public AtomNode<double> {
public:
  RealNode(const int &row, const int &col) : AtomNode<double>(row, col) {}
  RealNode(const int &row, const int &col, const double &v)
      : AtomNode<double>(row, col, v) {}
  string getHead() { return head_; }

private:
  string head_ = "Real";
};

class SymbolNode : public AtomNode<string> {
public:
  SymbolNode(const int &row, const int &col) : AtomNode<string>(row, col) {}
  SymbolNode(const int &row, const int &col, const string &v)
      : AtomNode<string>(row, col, v) {}
  SymbolNode(const int &row, const int &col, const char *v)
      : AtomNode<string>(row, col, string(v)) {}
  string getHead() const { return head_; }

private:
  string head_ = "Symbol";
};

class StringNode : public SymbolNode {
public:
  StringNode(const int &row, const int &col) : SymbolNode(row, col) {}
  StringNode(const int &row, const int &col, const string &v)
      : SymbolNode(row, col, v) {}
  StringNode(const int &row, const int &col, const char *v)
      : SymbolNode(row, col, string(v)) {}
  string getHead() const { return head_; }

  void toCCode_(ostringstream &o) { o << "\"" << getConstant() << "\""; }

private:
  string head_ = "String";
};
#endif /* __ATOM_H__ */
