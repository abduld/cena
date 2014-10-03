

#ifndef __NODE_H__
#define __NODE_H__

class Node;

class Node {
public:
  Node() {
    len_ = 0;
    parent_ = nullptr;
  }
  ~Node() {}
  virtual string toString() { return "unknown"; }
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
  shared_ptr<Node> parent_;
};

#endif /* __NODE_H__ */
