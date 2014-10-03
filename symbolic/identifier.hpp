
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

class Identifier : public Atom<string> {
public:
  Identifier() : Atom<string>() {}
  Identifier(string v) : Atom<string>(v) {}
  string getHead() { return head_; }
  void toCCode(ostringstream &o) { o << getConstant(); }
  void toString(ostringstream &o) { toCCode(o); }
  string toCCode() {
    ostringstream o;
    toCCode(o);
    return o.str();
  }
  string toString() {
    ostringstream o;
    toString(o);
    return o.str();
  }

private:
  string head_ = "Identifier";
  Type typ_;
};

#endif /* __IDENTIFIER_H__ */
