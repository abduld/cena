

#ifndef __COMPOUND_H__
#define __COMPOUND_H__

class Compound : public Node {
public:
  Compound() : Node() { vals_ = vector<shared_ptr<Node> >(); }
  virtual ~Compound() {
    // DEBUG;
  };
  Compound *operator<<=(const bool &val) {
    shared_ptr<Boolean> var(new Boolean(val));
    vals_.push_back(var);
    len_++;
    return this;
  }
  Compound &operator<<=(const int &val) {
    shared_ptr<Integer> var(new Integer(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  Compound &operator<<=(const int64_t &val) {
    shared_ptr<Integer> var(new Integer(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  Compound &operator<<=(const float &val) {
    shared_ptr<Real> var(new Real(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  Compound &operator<<=(const double &val) {
    shared_ptr<Real> var(new Real(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  Compound &operator<<=(const char *val) {
    shared_ptr<String> var(new String(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  Compound &operator<<=(const string &val) {
    shared_ptr<String> var(new String(val));
    vals_.push_back(var);
    len_++;
    return *this;
  }
  Compound &operator<<=(Compound *c) {
    vector<shared_ptr<Node> > vals = c->getValues();
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
  void push_back(shared_ptr<Node> var) {
    vals_.push_back(var);
    len_++;
    return;
  }
  string getHead() { return head_; }
  vector<shared_ptr<Node> > getValues() { return vals_; }

protected:
  int len_;
  string head_ = "Compound";
  vector<shared_ptr<Node> > vals_;
};

#endif /* __COMPOUND_H__ */