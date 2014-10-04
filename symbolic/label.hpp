
#ifndef __LABEL_H__
#define __LABEL_H__

class Label : public CompoundNode {
public:
  Label(const shared_ptr<Node> &node) : CompoundNode() { push_back(node); }
  ~Label() {}
  string getHead() { return head_; }

private:
  string head_ = "Label";
};

#endif /* __LABEL_H__ */
