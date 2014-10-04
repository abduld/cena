

#ifndef __ATOM_H__
#define __ATOM_H__

template <typename T> class AtomNode : public Node {
public:
  AtomNode() : Node() { val_ = (T)0; }
  AtomNode(T v) : Node() { val_ = v; }
  vector<shared_ptr<Node>> getValues() {
    vector<shared_ptr<Node>> vec;
    shared_ptr<Node> v(this);
    vec.push_back(v);
    return vec;
  }
  bool isAtomNode() const { return true; }
  T getConstant() const { return val_; }
  virtual void toCCode(ostringstream &o) { o << getConstant(); }
  virtual void toStringNode(ostringstream &o) { toCCode(o); }
  string toCCode() {
    ostringstream o;
    toCCode(o);
    return o.str();
  }
  string toStringNode() {
    ostringstream o;
    toStringNode(o);
    return o.str();
  }

private:
  T val_;
};

class BooleanNode : public AtomNode<bool> {
public:
  BooleanNode() : AtomNode<bool>() {}
  BooleanNode(bool v) : AtomNode<bool>(v) {}
  string getHead() { return head_; }
  void toCCode(ostringstream &o) { o << (getConstant() ? "true" : "false"); }

private:
  string head_ = "BooleanNode";
};

class IntegerNode : public AtomNode<int64_t> {
public:
  IntegerNode() : AtomNode<int64_t>() {}
  IntegerNode(int64_t v) : AtomNode<int64_t>(v) {}
  string getHead() { return head_; }

private:
  string head_ = "IntegerNode";
};

class RealNode : public AtomNode<double> {
public:
  RealNode() : AtomNode<double>() {}
  RealNode(double v) : AtomNode<double>(v) {}
  string getHead() { return head_; }

private:
  string head_ = "RealNode";
};

class StringNode : public AtomNode<string> {
public:
  StringNode() : AtomNode<string>() {}
  StringNode(string v) : AtomNode<string>(v) {}
  StringNode(const char *v) : AtomNode<string>(string(v)) {}
  string getHead() { return head_; }

private:
  string head_ = "StringNode";
};
#endif /* __ATOM_H__ */
