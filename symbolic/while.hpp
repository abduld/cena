

#ifndef __WHILE_H__
#define __WHILE_H__

class WhileNode : public CompoundNode {
  WhileNode() : CompoundNode() {}
  WhileNode(const shared_ptr<Node> &cond, const shared_ptr<Node> &body)
      : CompoundNode() {
    push_back(cond);
    push_back(body);
  }
  string getHead() const { return head_; }

private:
  string head_ = "WhileNode";
};
#endif /* __WHILE_H__ */
