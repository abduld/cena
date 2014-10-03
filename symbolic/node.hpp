

#ifndef __NODE_H__
#define __NODE_H__

class Node {
public:
  Node() {
    id_ = 0;
    len_ = 0;
    parent_ = nullptr;
    source_location_ = nullptr;
  }
  ~Node() {}
  void setId(const size_t &id) { id_ = id; }
  size_t getId() const { return id_; }
  virtual void toCCode(ostringstream &o) { o << "unknown"; }
  virtual void toString(ostringstream &o) { o << "unknown"; }
  virtual string toCCode() {
    ostringstream o;
    toCCode(o);
    return o.str();
  }
  virtual string toString() {
    ostringstream o;
    toString(o);
    return o.str();
  }
  virtual string getHead() { return "unknown"; }
  virtual bool isCompound() const { return false; }
  virtual bool isStatement() const { return false; }
  virtual bool isAtom() const { return false; }
  virtual bool isBlock() const { return false; }
  virtual vector<shared_ptr<Node> > getValues() {
    vector<shared_ptr<Node> > vec;
    return vec;
  }
  void setParent(shared_ptr<Node> parent) { parent_ = parent; }

protected:
  int len_;
  size_t id_;
  shared_ptr<Node> parent_;
  shared_ptr<Node> source_location_;
};

#endif /* __NODE_H__ */
