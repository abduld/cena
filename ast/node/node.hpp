

#ifndef __NODE_H__
#define __NODE_H__

class Node {
public:
  Node(const int &row, const int &col, const int &endrow, const int &endcol,
       const string &raw)
      : id_(0), label_(""), children_(), parent_(nullptr), row_(row), col_(col),
        endrow_(endrow), endcol_(endcol), raw_(raw) {}
  virtual ~Node() {}
  void setId(const size_t &id) { id_ = id; }
  size_t getId() const { return id_; }
  virtual void toCCode_(ostringstream &o) {
    o << "unkown(" << this->getHead() << ")";
  }
  virtual void toString_(ostringstream &o) {
    o << "unkown(" << this->getHead() << ")";
  }
  Json getLocation() {

    Json::object obj;
    Json::object start;
    Json::object end;

    start["line"] = row_;
    start["column"] = col_;

    end["line"] = endrow_;
    end["column"] = endcol_;

    obj["start"] = start;
    obj["end"] = end;

    return obj;
  }
  virtual Json toJSON_() { return Json(); }
  virtual Json toEsprima_() { return Json(); }
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
  Json toJSON() { return toJSON_(); }
  string toJSONString() { return toJSON().dump(); }
  Json toEsprima() { return toEsprima_(); }
  string toEsprimaString() { return toEsprima().dump(); }

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
  virtual string getHead() const { return "unknown"; }
  virtual bool isNode() const { return false; }
  virtual bool isStatement() const { return false; }
  virtual bool isCompound() const { return false; }
  virtual bool isAtom() const { return false; }
  virtual bool isBlock() const { return false; }
  virtual bool isProgram() const { return false; }
  virtual bool isEmpty() const { return false; }
  virtual bool isSkip() const { return false; }
  virtual vector<shared_ptr<Node>> getValues() {
    vector<shared_ptr<Node>> vec;
    return vec;
  }
  void setParent(Node *parent) { parent_ = parent; }
  bool hasParent() { return parent_ != nullptr; }
  virtual bool hasChildren() const { return children_.size() != 0; }
  virtual vector<shared_ptr<Node>> getChildren() { return children_; }
  void addChild(const shared_ptr<Node> &child) { children_.push_back(child); }

  // void accept(ASTVisitor *visitor) { visitor->visit(this); }
  virtual void traverse(ASTVisitor *visitor) { visitor->visit(this); }

protected:
  size_t id_;
  vector<shared_ptr<Node>> children_{};
  Node *parent_ = nullptr;
  int row_{};
  int col_{};
  int endrow_{};
  int endcol_{};
  string raw_{};
  string label_{};
};

template <typename T> static constexpr bool is_compound(const T &nd) {
  return false;
}

template <typename T> static constexpr bool is_statement(const T &nd) {
  return false;
}

template <typename T> static constexpr bool is_atomic(const T &nd) {
  return false;
}

#endif /* __NODE_H__ */
