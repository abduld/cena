
#ifndef __CHARLIT_H__
#define __CHARLIT_H__

class CharacterNode : public AtomNode<unsigned char> {
public:
  CharacterNode(const int &row, const int &col)
      : AtomNode<unsigned char>(row, col) {}
  CharacterNode(const int &row, const int &col, const unsigned char &v)
      : AtomNode<unsigned char>(row, col, v) {}
  string getHead() const { return head_; }
  void toCCode_(ostringstream &o) {
    o << "'" << string(1, getConstant()) << "'";
  }
  void traverse(ASTVisitor * visitor) {
      accept(visitor);
  }

private:
  string head_ = "Character";
};

#endif /* __CHARLIT_H__ */
