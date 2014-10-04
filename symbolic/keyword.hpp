
#ifndef __KEYWORD_H__
#define __KEYWORD_H__

class Keyword : public Node {
public:
  Keyword(string head) : Node(), head_(head) {}
  ~Keyword() {}
  string getHead() { return head_; }

protected:
  string head_;
};

#endif /* __KEYWORD_H__ */
