

#ifndef __FOR_H__
#define __FOR_H__

class ForNode : public Node {
public:
  ForNode(const int &row, const int &col) : Node(row, col) {}
  string getHead() const { return head_; }
  void setInit(const shared_ptr<Node> &cond) { init_ = cond; }
  void setCond(const shared_ptr<Node> &nd) { cond_ = nd; }
  void setInc(const shared_ptr<Node> &nd) { inc_ = nd; }
  void setBody(const shared_ptr<Node> &nd) {
    if (body_ == nullptr) {
      body_ = shared_ptr<BlockNode>(new BlockNode(row_, col_));
    }
    *body_ <<= nd;
  }
  void toCCode_(ostringstream &o) {
    o << "for (";
    if (init_ != nullptr) {
      init_->toCCode_(o);
    }
    o << ",";
    if (cond_ != nullptr) {
      cond_->toCCode_(o);
    }
    o << ",";
    if (inc_ != nullptr) {
      inc_->toCCode_(o);
    }
    o << ") ";
    body_->toCCode_(o);
  }
  void toString_(ostringstream &o) { toCCode_(o); }
  bool isBlock() const { return true; }
  void toJSON_(ostringstream &o) { o << "{\"type\": \"unknown\"}"; }

private:
  string head_ = "For";
  shared_ptr<Node> init_ = nullptr;
  shared_ptr<Node> cond_ = nullptr;
  shared_ptr<Node> inc_ = nullptr;
  shared_ptr<BlockNode> body_ = nullptr;
};
#endif /* __FOR_H__ */
