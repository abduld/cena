

#ifndef __REFERENCE_H__
#define __REFERENCE_H__

class ReferenceNode : public Node {
public:
  ReferenceNode(const int &line, const int &col) : Node(line, col) {}
  ReferenceNode(const int &line, const int &col, const shared_ptr<Node> &typ)
      : Node(line, col) {
    type_ = typ;
  }
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) {
    if (dynamic_cast<TypeNode>(type_.get())) {
      toCCode_(o);
      o << "*";
    } else {
      o << "*";
      toCCode_(o);
    }
  }
  void toString_(ostringstream &o) {
    if (dynamic_cast<TypeNode>(type_.get())) {
      toString_(o);
      o << "*";
    } else {
      o << "*";
      toString_(o);
    }
  }

private:
  string head_ = "ReferenceNode";
  shared_ptr<Node> type_;
};
#endif /* __REFERENCE_H__ */
