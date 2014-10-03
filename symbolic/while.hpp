

#ifndef __WHILE_H__
#define __WHILE_H__

class While : public Compound {
  While() : Compound() {}
  While(const shared_ptr<Node> &cond, const shared_ptr<Node> &body)
      : Compound() {
    push_back(cond);
    push_back(body);
  }
  string getHead() const { return head_; }

private:
  string head_ = "While";
};
#endif /* __WHILE_H__ */