
#ifndef __LABEL_H__
#define __LABEL_H__

class Label : public Node {
public:
  Label(const int &row, const int &col, const shared_ptr<Node> &node)
      : Node(row, col), lbl_(node) {}
  ~Label() {}
  string getHead() { return head_; }

private:
  string head_ = "Label";
  shared_ptr<Node> lbl_;
};

#endif /* __LABEL_H__ */