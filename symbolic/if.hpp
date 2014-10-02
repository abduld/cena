

#ifndef __IF_H__
#define __IF_H__

class If : public Compound {
  If() : Compound() {}
  If(Node &cond, Node &thenPart) : Compound() {
    push(cond);
    push(thenPart);
  }
  If(Node &cond, Node &thenPart, Node &elsePart) : Compound() {
    push(cond);
    push(thenPart);
    push(elsePart);
  }
  string getHead() const { return head_; }

private:
  string head_ = "if";
};
#endif /* __CIF_H__ */