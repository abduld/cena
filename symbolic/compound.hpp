

#ifndef __COMPOUND_H__
#define __COMPOUND_H__

class CompoundNode : public Node {
public:
  CompoundNode() : Node() { vals_ = vector<shared_ptr<Node>>(); }
  virtual ~CompoundNode() { vals_.clear(); };
  bool isCompoundNode() const { return true; }
  CompoundNode *operator<<=(const bool &val) {
    shared_ptr<BooleanNode> var(new BooleanNode(val));
    vals_.push_back(var);
    len_++;
    return this;
  }
  CompoundNode &operator<<=(const int &val) {
    shared_ptr<IntegerNode> var(new IntegerNode(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(const int64_t &val) {
    shared_ptr<IntegerNode> var(new IntegerNode(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(const float &val) {
    shared_ptr<RealNode> var(new RealNode(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(const double &val) {
    shared_ptr<RealNode> var(new RealNode(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(const char *val) {
    shared_ptr<StringNode> var(new StringNode(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(const string &val) {
    shared_ptr<StringNode> var(new StringNode(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(const shared_ptr<Node> &c) {
    vals_.push_back(c);
    len_++;
    return *this;
  }
  CompoundNode &operator<<=(CompoundNode *c) {
    vector<shared_ptr<Node>> vals = c->getValues();
    for (auto iter = vals.begin(); iter != vals.end(); iter++) {
      vals_.push_back(*iter);
    }
    len_ += vals.size();
    return *this;
  }
  void push_back(Node *v) {
    shared_ptr<Node> var(v);
    vals_.push_back(var);
    len_++;
    return;
  }
  void push_back(const shared_ptr<Node> &var) {
    vals_.push_back(var);
    len_++;
    return;
  }
  void push_back(const vector<shared_ptr<Node>> &var) {
    for (auto iter : var) {
      push_back(iter);
    }
    return;
  }
  shared_ptr<Node> getPart(int idx) { return vals_[idx]; }
  void setPart(int idx, const shared_ptr<Node> &var) {
    if (vals_.size() <= idx) {
      vals_.resize(idx + 1);
    }
    vals_[idx] = var;
    return;
  }
  virtual string getHead() { return head_; }
  vector<shared_ptr<Node>> getValues() { return vals_; }

protected:
  int len_;
  string head_ = "CompoundNode";
  vector<shared_ptr<Node>> vals_;
};

#endif /* __COMPOUND_H__ */
