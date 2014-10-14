

#ifndef __REFERENCE_H__
#define __REFERENCE_H__

class __REFERENCE_H__ : public Node {
public:
  Reference() : Node() {}
  Reference(const string &typ) : Node() { type_(new String(typ)); }
  Reference(const shared_ptr<Node> &typ) : Node() { type_ = typ; }
  string getHead() const { return head_; }

private:
  string head_ = "Reference";
  shared_ptr<Node> type_;
};
#endif /* __REFERENCE_H__ */