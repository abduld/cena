

#ifndef __PROGRAM_H__
#define __PROGRAM_H__

class ProgramNode : public CompoundNode {
public:
  ProgramNode() : CompoundNode(0, 0, 0, 0, "") {}
  ~ProgramNode() {}
  string getHead() { return head_; }
  bool isProgram() const { return true; }
  Json toEsprima_() {
    Json::object obj;
    std::vector<Json> lst;
    obj["type"] = "Program";
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    for (auto elem : vals_) {
      lst.push_back(elem->toEsprima_());
    }

    obj["body"] = lst;
    return Json(obj);
  }

private:
  string head_ = "Program";
};

#endif /* __PROGRAM_H__ */
