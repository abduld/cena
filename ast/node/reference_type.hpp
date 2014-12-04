

#ifndef __REFERENCE_TYPE_H__
#define __REFERENCE_TYPE_H__

class ReferenceTypeNode : public TypeNode {
public:
  ReferenceTypeNode(const int &row, const int &col, const int &endrow,
                    const int &endcol, const string &raw)
      : TypeNode(row, col, endrow, endcol, raw) {}
  ReferenceTypeNode(const int &row, const int &col, const int &endrow,
                    const int &endcol, const string &raw,
                    const shared_ptr<TypeNode> &typ)
      : TypeNode(row, col, endrow, endcol, raw), type_(typ) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) {
    type_->toCCode_(o);
    o << "*";
  }
  void toString_(ostringstream &o) {
    type_->toString_(o);
    o << "*";
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "ReferenceType";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = type_->toEsprima_();
    return obj;
  }

private:
  string head_ = "ReferenceType";
  shared_ptr<TypeNode> type_ = nullptr;
};
#endif /* __REFERENCE_TYPE_H__ */
