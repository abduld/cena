

#ifndef __IF_H__
#define __IF_H__

class If : public Compound {
  If() : Compound() {}
  If(const shared_ptr<Node> &cond, const shared_ptr<Node> &thenPart)
      : Compound() {
    push_back(cond);
    push_back(thenPart);
  }
  If(const shared_ptr<Node> &cond, const shared_ptr<Node> &thenPart,
     const shared_ptr<Node> &elsePart)
      : Compound() {
    push_back(cond);
    push_back(thenPart);
    push_back(elsePart);
  }
  string getHead() const { return head_; }

private:
  string head_ = "If";
};
#endif /* __IF_H__ */