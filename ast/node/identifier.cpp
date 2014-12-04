
#include "ast.hpp"

Json IdentifierNode::toEsprima_() {
  Json::object obj;
  obj["type"] = "Identifier";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
  obj["name"] = getName();
  if (typ_ != nullptr) {
    obj["kind"] = typ_->toEsprima_();
  }
  return obj;
}
void IdentifierNode::traverse(ASTVisitor * visitor) {
	if (typ_ != nullptr) {
      typ_->traverse(visitor);
  }
}


