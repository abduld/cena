

#ifndef __TYPE_H__
#define __TYPE_H__

class Type : public Node {
  Type() : Compound() {}
  Type( const shared_ptr<Node> & cond, const shared_ptr<Node> & body) : Compound() {
      push_back(cond);
      push_back(body);
  }
  string getHead() const { return head_; }

private:
  string head_ = "Type";
  vector<shared_ptr<Node>> qualifiers_;
  vector<shared_ptr<Node>> base_;
  vector<shared_ptr<Node>> address_space_;
};
#endif /* __TYPE_H__ */