
#ifndef __CALL_H__
#define __CALL_H__

class Call : public Compound {
public:
  Call() : Compound() {}
  Call(shared_ptr<Node> fun, shared_ptr<Node> arg) : Compound() {
    vector<shared_ptr<Node> > args;
    args.push_back(arg);
    push_back(fun);
    push_back(args);
  }
  Call(shared_ptr<Node> fun, vector<shared_ptr<Node> > args) : Compound() {
    push_back(fun);
    push_back(args);
  }
  ~Call() {}
  shared_ptr<Node> getFunction() { getPart(0); }
  vector<shared_ptr<Node> > getArgs() {
    vector<shared_ptr<Node> > vals = getValues();
    vector<shared_ptr<Node> > res =
        vector<shared_ptr<Node> >(vals.begin() + 1, vals.end());
    return res;
  }
  string getHead() { return head_; }
  void toCCode(ostringstream &o) {
    getFunction()->toCCode(o);
    o << "(";
    RiffleJoin(o, ToCCode(vals_), string(" ,"));
    o << ")";
  }

private:
  string head_ = "Call";
};

#endif /* __CALL_H__ */
