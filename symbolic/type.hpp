

#ifndef __TYPE_H__
#define __TYPE_H__

class Type : public Node {
public:
  Type() : Node() {}
  void addQualifyer(const string &qual) {
    addQualifyer(shared_ptr<String>(new String(qual)));
  }
  void addQualifyer(const shared_ptr<Node> &qual) {
    qualifiers_.push_back(qual);
  }
  void addBase(const string &base) {
    addQualifyer(shared_ptr<String>(new String(base)));
  }
  void addBase(const shared_ptr<Node> &base) { base_.push_back(base); }
  void addAddressSpace(const string &addr) {
    addQualifyer(shared_ptr<String>(new String(addr)));
  }
  void addAddressSpace(const shared_ptr<Node> &addr) {
    address_space_.push_back(addr);
  }
  vector<shared_ptr<Node> > getQualifiers() { return qualifiers_; }
  vector<shared_ptr<Node> > getBase() { return base_; }
  vector<shared_ptr<Node> > getAddressSpace() { return address_space_; }
  string getHead() const { return head_; }

private:
  string head_ = "Type";
  vector<shared_ptr<Node> > qualifiers_;
  vector<shared_ptr<Node> > base_;
  vector<shared_ptr<Node> > address_space_;
};
#endif /* __TYPE_H__ */