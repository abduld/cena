

#ifndef __REFERENCE_H__
#define __REFERENCE_H__

class ReferenceNode : public Node {
public:
  ReferenceNode(const int &line, const int &col, const int &endrow,
                const int &endcol, const string &raw)
      : Node(line, col) {}
  ReferenceNode(const int &line, const int &col, const int &endrow,
                const int &endcol, const string &raw,
                const shared_ptr<Node> &typ)
      : Node(line, col) {
    type_ = typ;
  }
  string getHead() const override { return head_; }
  void toCCode_(ostringstream &o) override {
    if (dynamic_cast<TypeNode>(type_.get())) {
      toCCode_(o);
      o << "*";
    } else {
      o << "*";
      toCCode_(o);
    }
  }
  void toString_(ostringstream &o) override {
    if (dynamic_cast<TypeNode>(type_.get())) {
      toString_(o);
      o << "*";
    } else {
      o << "*";
      toString_(o);
    }
  }
  Json toEsprima_() override {
    Json::object obj;
    obj["type"] = "ReferenceExpression";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = type_->toEsprima_();
    return obj;
  }

private:
  string head_ = "ReferenceNode";
  shared_ptr<Node> type_;
};
#endif /* __REFERENCE_H__ */
