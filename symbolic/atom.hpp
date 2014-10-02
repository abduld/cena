

#ifndef __ATOM_H__
#define __ATOM_H__

template <typename T> class Atom : public Node {
protected:
  Atom() : Node() { val_ = (T)0; }
  Atom(T v) : Node() { val_ = v; }
  vector<shared_ptr<Node> > getValues() {
    vector<shared_ptr<Node> > vec;
    shared_ptr<Node> v(this);
    vec.push_back(v);
    return vec;
  }
  T getConstant() const { return val_; }

private:
  T val_;
};

class Boolean : public Atom<bool> {
public:
  Boolean() : Atom<bool>() {}
  Boolean(bool v) : Atom<bool>(v) {}
  string getHead() { return head_; }
  string toString() { return getConstant() ? "true" : "false"; }

private:
  string head_ = "Boolean";
};

class Integer : public Atom<int64_t> {
public:
  Integer() : Atom<int64_t>() {}
  Integer(int64_t v) : Atom<int64_t>(v) {}
  string getHead() { return head_; }
  string toString() {
    std::ostringstream o;
    o << getConstant();
    return o.str();
  }

private:
  string head_ = "Integer";
};

class Real : public Atom<double> {
public:
  Real() : Atom<double>() {}
  Real(double v) : Atom<double>(v) {}
  string getHead() { return head_; }
  string toString() {
    std::ostringstream o;
    o << getConstant();
    return o.str();
  }

private:
  string head_ = "Real";
};

class String : public Atom<string> {
public:
  String() : Atom<string>() {}
  String(string v) : Atom<string>(v) {}
  String(const char * v) : Atom<string>(string(v)) {}
  string getHead() { return head_; }
  string toString() { return getConstant(); }

private:
  string head_ = "String";
};
#endif /* __ATOM_H__ */