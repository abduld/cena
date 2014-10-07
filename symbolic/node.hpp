

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
  virtual void toCCode_(ostringstream &o) {
    o << "unkown(" << this->getHead() << ")";
  }
  virtual void toString_(ostringstream &o) {
    o << "unkown(" << this->getHead() << ")";
  }
  virtual void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  string toCCode() {
    ostringstream o;
    toCCode_(o);
    return o.str();
  }
  string toString() {
    ostringstream o;
    toString_(o);
    return o.str();
  }
  string toJSON() {
    ostringstream o;
    toJSON_(o);
    return o.str();
  }

  virtual Node *operator<<=(const bool &val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return this;
  }
  virtual Node &operator<<=(const int &val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(const int64_t &val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(const float &val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(const double &val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(const char *val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(const string &val) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(const shared_ptr<Node> &c) {
    std::cerr << "undefined <<= \n" << std::endl;
    return *this;
  }
  virtual Node &operator<<=(Node *c) { return *this; }
  virtual string getHead() { return "unknown"; }
  virtual bool isNode() const { return false; }
  virtual bool isStatement() const { return false; }
  virtual bool isCompound() const { return false; }
  virtual bool isAtom() const { return false; }
  virtual bool isBlock() const { return false; }
  virtual bool isProgram() const { return false; }
  virtual bool isEmpty() const { return false; }
  virtual vector<shared_ptr<Node>> getValues() {
    vector<shared_ptr<Node>> vec;
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
