
#ifndef __CASE_H__
#define __CASE_H__

class Case : public Compound {
public:
  Case(const shared_ptr<Node> &node) : Compound() { push_back(node); }
  ~Case() {}
  string getHead() { return head_; }

private:
  string head_ = "Case";
};

#endif /* __CASE_H__ */
