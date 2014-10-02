
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__


class Identifier : public Atom<string> {
public:
  Identifier() : Atom<string>() {}
  Identifier(string v) : Atom<string>(v) {}
  string getHead() { return head_; }
  string toString() { return getConstant(); }

private:
  string head_ = "Identifier";
};

#endif /* __IDENTIFIER_H__ */
