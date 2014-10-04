
#ifndef __CALL_H__
#define __CALL_H__

class Call : public CompoundNode {
public:
  Call() : CompoundNode() {}
  Call(shared_ptr<Node> fun, shared_ptr<Node> arg) : CompoundNode() {
    vector<shared_ptr<Node>> args;
    args.push_back(arg);
    push_back(fun);
    push_back(args);
  }
  Call(shared_ptr<Node> fun, vector<shared_ptr<Node>> args) : CompoundNode() {
    push_back(fun);
    push_back(args);
  }
  ~Call() {}
  shared_ptr<Node> getFunction() { return getPart(0); }
  vector<shared_ptr<Node>> getArgs() {
    vector<shared_ptr<Node>> vals = getValues();
    vector<shared_ptr<Node>> res =
        vector<shared_ptr<Node>>(vals.begin() + 1, vals.end());
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
