
#ifndef __CALL_H__
#define __CALL_H__

class CallNode : public Node {
public:
  CallNode(const int &row, const int &col) : Node(row, col) {}
  CallNode(const int &row, const int &col,
           const shared_ptr<IdentifierNode> &fun,
           const vector<shared_ptr<Node> > &args)
      : Node(row, col), fun_(fun), args_(new CompoundNode(row, col, args)) {}
  ~CallNode() {}
  shared_ptr<IdentifierNode> getFunction() const { return fun_; }
  vector<shared_ptr<Node> > getArgs() const { return args_->getValues(); }
  void setFunction(const shared_ptr<IdentifierNode> &fun) { fun_ = fun; }
  void addArg(const shared_ptr<Node> &nd) {
    if (args_ == nullptr) {
      args_ = shared_ptr<CompoundNode>(new CompoundNode(row_, col_));
    }
    args_->push_back(nd);
  }
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) {
    fun_->toCCode_(o);
    o << "(";
    if (args_ != nullptr) {
      args_->toCCode_(o);
    }
    o << ")";
  }
  void toString_(ostringstream &o) {
    fun_->toString_(o);
    o << "(";
    if (args_ != nullptr) {
      args_->toString_(o);
    }
    o << ")";
  }
  Json toEsprima_() override {
    Json::object obj;
    vector<Json> args;
    obj["type"] = "CallExpression";
    obj["line"] = row;
    obj["column"] = column;
    obj["callee"] = fun_->toString();
    if (args_ != nullptr) {
      for (auto arg : args_->getValues()) {
        args.push_back(arg->toEsprima_());
      }
    }
    obj["arguments"] = args;
    return obj;
  }

private:
  string head_ = "Call";
  shared_ptr<IdentifierNode> fun_ = nullptr;
  shared_ptr<CompoundNode> args_ = nullptr;
};

#endif /* __CALL_H__ */
