

#ifndef __COMPOUND_H__
#define __COMPOUND_H__

#include "utilities.hpp"

class CompoundNode : public Node, public NodeAcceptor<CompoundNode> {
public:
  CompoundNode(const int &row, const int &col, const int &endrow,
               const int &endcol, const string &raw)
      : Node(row, col, endrow, endcol, raw) {
    vals_ = vector<shared_ptr<Node>>();
  }
  CompoundNode(const int &row, const int &col, const int &endrow,
               const int &endcol, const string &raw,
               const vector<shared_ptr<Node>> &vals)
      : Node(row, col, endrow, endcol, raw), vals_(vals) {}
  virtual ~CompoundNode() { vals_.clear(); }
  virtual bool isCompound() const { return true; }
  virtual bool isEmpty() const { return getArgCount() == 0; }
  CompoundNode *operator<<=(const bool &val) {
    shared_ptr<BooleanNode> var(new BooleanNode(row_, col_, endrow_, endcol_, val ? "true" : "false", val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return this;
  }
  CompoundNode &operator<<=(const int &val) {
    shared_ptr<IntegerNode> var(new IntegerNode(row_, col_, endrow_, endcol_, convertToString(val), val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return *this;
  }
  CompoundNode &operator<<=(const int64_t &val) {
    shared_ptr<IntegerNode> var(new IntegerNode(row_, col_,endrow_, endcol_, convertToString(val), val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return *this;
  }
  CompoundNode &operator<<=(const float &val) {
    shared_ptr<RealNode> var(new RealNode(row_, col_, endrow_, endcol_, convertToString(val), val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return *this;
  }
  CompoundNode &operator<<=(const double &val) {
    shared_ptr<RealNode> var(new RealNode(row_, col_, endrow_, endcol_, convertToString(val), val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return *this;
  }
  CompoundNode &operator<<=(const char *val) {
    shared_ptr<StringNode> var(new StringNode(row_, col_, endrow_, endcol_, convertToString(val), val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return *this;
  }
  CompoundNode &operator<<=(const string &val) {
    shared_ptr<StringNode> var(new StringNode(row_, col_, endrow_, endcol_, convertToString(val), val));
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return *this;
  }
  CompoundNode &operator<<=(const shared_ptr<Node> &c) {
    if (!vals_.empty() && vals_.back() == c) {
      return *this;
    }
    if (c->isCompound()) {
      *this <<= std::static_pointer_cast<CompoundNode>(c);
    } else {
      c->setParent(this);
      addChild(c);
      vals_.push_back(c);
    }
    return *this;
  }
  CompoundNode &operator<<=(CompoundNode *c) {

    vector<shared_ptr<Node>> vals = c->getValues();
    for (auto iter = vals.begin(); iter != vals.end(); iter++) {
      vals_.push_back(*iter);
    }
    return *this;
  }
  CompoundNode &operator<<=(const shared_ptr<CompoundNode> &c) {
    if (!vals_.empty() && vals_.back() == c) {
      return *this;
    }
    vector<shared_ptr<Node>> vals = c->getValues();
    for (auto iter = vals.begin(); iter != vals.end(); iter++) {
      vals_.push_back(*iter);
    }
    return *this;
  }
  bool isListInitialization(bool val) {
    isListInitialization_ = val;
    return isListInitialization_;
  }
  bool isListInitialization() { return isListInitialization_; }
  void push_back(Node *v) {
    shared_ptr<Node> var(v);
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return;
  }
  void push_back(const shared_ptr<Node> &var) {
    if (!vals_.empty() && vals_.back() == var) {
      return;
    }
    var->setParent(this);
    addChild(var);
    vals_.push_back(var);
    return;
  }
  void push_back(const vector<shared_ptr<Node>> &var) {
    for (auto iter : var) {
      iter->setParent(this);
      addChild(iter);
      push_back(iter);
    }
    return;
  }
  size_t getArgCount() const { return vals_.size(); }
  shared_ptr<Node> getPart(int idx) const { return vals_[idx]; }
  void setPart(int idx, const shared_ptr<Node> &var) {
    if (vals_.size() <= idx) {
      vals_.resize(idx + 1);
    }
    vals_[idx] = var;
    return;
  }
  virtual string getHead() const override { return head_; }
  vector<shared_ptr<Node>> getValues() { return vals_; }

  virtual void toCCode_(ostringstream &o) {
    auto vals = getValues();
    auto len = vals.size();
    if (isBlock()) {
      o << "{\n";
    }
    if (isListInitialization()) {
      o << "{";
    }
    if (!isEmpty()) {
      for (auto v : vals) {
        len--;
        v->toCCode_(o);
        if (v->isBlock() || v->isSkip()) {
          continue;
        }
        if (v->isStatement() || isBlock()) {
          o << ";";
          o << " /* " << v->getHead() << "*/";
          o << "\n";
        } else if (!isProgram() && len > 0) {
          o << " /* " << v->getHead() << "*/";
          o << ", ";
        }
      }
    }
    if (isBlock()) {
      o << "}\n";
    }
    if (isListInitialization()) {
      o << "}";
    }
  }
  virtual Json toEsprima_() {

    std::vector<Json> lst;
    for (auto elem : vals_) {
      lst.push_back(elem->toEsprima_());
    }
    if (isListInitialization()) {

      Json::object obj;
      vector<Json> args;
      obj["type"] = "ArrayExpression";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
      obj["elements"] = lst;
      return obj;
    } else {
      return Json(lst);
    }
  }
  virtual void toString_(ostringstream &o) {
    auto vals = getValues();
    auto len = vals.size();
    if (isBlock()) {
      o << "{\n";
    }
    if (isListInitialization()) {
      o << "{";
    }
    if (!isEmpty()) {
      for (auto v : vals) {
        len--;
        v->toString_(o);
        if (v->isBlock()) {
          continue;
        }
        if (v->isStatement() || isBlock()) {
          o << "\n";
        } else if (!isProgram() && len > 0) {
          o << ", ";
        }
      }
    }
    if (isBlock()) {
      o << "}\n";
    }
    if (isListInitialization()) {
      o << "}";
    }
  }
  virtual void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }

  virtual void traverse(ASTVisitor *visitor) override {
    for (auto elem : vals_) {
      elem->traverse(visitor);
    }
  }

protected:
  string head_ = "CompoundNode";
  vector<shared_ptr<Node>> vals_;
  bool isListInitialization_{false};
};

#endif /* __COMPOUND_H__ */
