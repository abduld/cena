
#include "ast.hpp"

Json IdentifierNode::toEsprima_() {
  Json::object obj;
  obj["type"] = "Identifier";
  obj["line"] = row_;
  obj["column"] = col_;
  obj["name"] = getName();
  if (typ_ != nullptr) {
    obj["kind"] = typ_->toEsprima_();
  }
  return obj;
}
