

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
  void setConstant(const T & val)  {  val_ = val; }
  T getConstant() const { return val_; }
  virtual void toCCode_(ostringstream &o) {
    // std::cout << getHead() << "   ::  " << getConstant() << std::endl;
    o << getConstant();
  }
  virtual void toString_(ostringstream &o) { toCCode_(o); }
  virtual void toJSON_(ostringstream &o) { toCCode_(o); }

private:
  T val_;
};

class BooleanNode : public AtomNode<bool> {
public:
  BooleanNode() : AtomNode<bool>() {}
  BooleanNode(bool v) : AtomNode<bool>(v) {}
  string getHead() { return head_; }
  void toCCode_(ostringstream &o) { o << (getConstant() ? "true" : "false"); }

private:
  string head_ = "Boolean";
};

class CharacterNode : public AtomNode<unsigned char> {
public:
  CharacterNode() : AtomNode<unsigned char>() {}
  CharacterNode(unsigned char v) : AtomNode<unsigned char>(v) {}
  string getHead() { return head_; }
  void toCCode_(ostringstream &o) {
    o << "'" << string(1, getConstant()) << "'";
  }

private:
  string head_ = "Character";
};
class IntegerNode : public virtual AtomNode<int64_t> {
public:
  IntegerNode() : AtomNode<int64_t>() {}
  IntegerNode(int64_t v) : AtomNode<int64_t>(v) {}
  string getHead() { return head_; }

private:
  string head_ = "Integer";
};

class RealNode : public AtomNode<double> {
public:
  RealNode() : AtomNode<double>() {}
  RealNode(double v) : AtomNode<double>(v) {}
  string getHead() { return head_; }

private:
  string head_ = "Real";
};

class StringNode : public AtomNode<string> {
public:
  StringNode() : AtomNode<string>() {}
  StringNode(string v) : AtomNode<string>(v) {}
  StringNode(const char *v) : AtomNode<string>(string(v)) {}
  string getHead() { return head_; }

private:
  string head_ = "String";
};
#endif /* __ATOM_H__ */
