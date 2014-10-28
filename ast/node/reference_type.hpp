


#ifndef __REFERENCE_TYPE_H__
#define __REFERENCE_TYPE_H__

class ReferenceTypeNode : public TypeNode {
public:
  ReferenceTypeNode(const int &row, const int &col) : TypeNode(row, col) {}
  ReferenceTypeNode(const int &row, const int &col, const shared_ptr<TypeNode> &typ) : TypeNode(row, col), type_(typ) {
  }
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) {
		  type_->toCCode_(o);
		  o << "*";
  }
  void toString_(ostringstream &o) {
		  type_->toString_(o);
		  o << "*";
  }

private:
  string head_ = "ReferenceType";
  shared_ptr<TypeNode> type_ = nullptr;
};
#endif /* __REFERENCE_TYPE_H__ */
