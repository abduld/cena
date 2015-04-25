

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
  virtual string getHead() const {
    return "Node";
  }
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
  
  bool needsNewLine() const { return needsNewLine_; }
  bool isNode() const { return isNode_; }
  bool isStatement() const { return isStatement_; }
  bool isCompound() const { return isCompound_; }
  bool isAtom() const { return isAtom_; }
  bool isBlock() const { return isBlock_; }
  bool isArgument() const { return isArgument_; }
  bool isProgram() const { return isProgram_; }
  bool isEmpty() const { return isEmpty_; }
  bool isSkip() const { return isSkip_; }

  bool needsNewLine(const bool & val) { needsNewLine_ = val; return needsNewLine_; }
  bool isNode(const bool & val) { isNode_ = val; return isNode_; }
  bool isStatement(const bool & val) { isStatement_ = val; return isStatement_; }
  bool isCompound(const bool & val) { isCompound_ = val; return isCompound_; }
  bool isAtom(const bool & val) { isAtom_ = val; return isAtom_; }
  bool isBlock(const bool & val) { isBlock_ = val; return isBlock_; }
  bool isArgument(const bool & val) { isArgument_ = val; return isArgument_; }
  bool isProgram(const bool & val) { isProgram_ = val; return isProgram_; }
  bool isEmpty(const bool & val) { isEmpty_ = val; return isEmpty_; }
  bool isSkip(const bool & val) { isSkip_ = val; return isSkip_; }

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
  
  bool needsNewLine_{false};
  bool isNode_{false};
  bool isStatement_{false};
  bool isCompound_{false};
  bool isAtom_{false};
  bool isBlock_{false};
  bool isArgument_{false};
  bool isProgram_{false};
  bool isEmpty_{false};
  bool isSkip_{false};
};


#endif /* __NODE_H__ */
