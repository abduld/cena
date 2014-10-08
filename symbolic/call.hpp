
#ifndef __CALL_H__
#define __CALL_H__

class CallNode : public Node {
public:
  CallNode() : Node() {}
  CallNode(const shared_ptr<IdentifierNode> &fun,
           const vector<shared_ptr<Node>> &args)
      : Node(), fun_(fun), args_(new CompoundNode(args)) {}
  ~CallNode() {}
  shared_ptr<IdentifierNode> getFunction() { return fun_; }
  vector<shared_ptr<Node>> getArgs() { return args_->getValues(); }
  void setFunction(const shared_ptr<IdentifierNode> &fun) { fun_ = fun; }
  void addArg(const shared_ptr<Node> &nd) {
    if (args_ == nullptr) {
      args_ = shared_ptr<CompoundNode>(new CompoundNode());
    }
    args_->push_back(nd);
  }
  string getHead() { return head_; }
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

private:
  string head_ = "Call";
  shared_ptr<IdentifierNode> fun_ = nullptr;
  shared_ptr<CompoundNode> args_ = nullptr;
};

#endif /* __CALL_H__ */
