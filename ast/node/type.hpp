

#ifndef __TYPE_H__
#define __TYPE_H__

class TypeNode : public Node {
public:
  TypeNode(const int &row, const int &col) : Node(row, col) {}
  TypeNode(const int &row, const int &col, const string &typ) : Node(row, col) {
    addBase(typ);
  }
  void addQualifyer(const string &qual) {
    addQualifyer(shared_ptr<IdentifierNode>(new IdentifierNode(row_, col_, qual)));
  }
  void addQualifyer(const shared_ptr<Node> &qual) {
    qualifiers_.push_back(qual);
  }
  void addBase(const string &base) {
    addBase(shared_ptr<IdentifierNode>(new IdentifierNode(row_, col_, base)));
  }
  void addBase(const shared_ptr<Node> &base) { base_.push_back(base); }
  void addAddressSpace(const string &addr) {
    addAddressSpace(shared_ptr<IdentifierNode>(new IdentifierNode(row_, col_, addr)));
  }
  void addAddressSpace(const shared_ptr<Node> &addr) {
    address_space_.push_back(addr);
  }
  vector<shared_ptr<Node>> getQualifiers() const { return qualifiers_; }
  vector<shared_ptr<Node>> getBase() const { return base_; }
  vector<shared_ptr<Node>> getAddressSpace() const { return address_space_; }

  void toCCode_(ostringstream &o) {
    for (auto addr : address_space_) {
      addr->toCCode_(o);
      o << " ";
    }
    for (auto qual : qualifiers_) {
      qual->toCCode_(o);
      o << " ";
    }
    for (auto base : base_) {
      base->toCCode_(o);
      o << " ";
    }
  }
  void toString_(ostringstream &o) { toCCode_(o); }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }
  string getHead() const { return head_; }

private:
  string head_ = "Type";
  vector<shared_ptr<Node>> qualifiers_{};
  vector<shared_ptr<Node>> base_{};
  vector<shared_ptr<Node>> address_space_{};
};
#endif /* __TYPE_H__ */
