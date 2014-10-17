
#include "ast.hpp"

AssignNode::AssignNode(const int &row, const int &col) : Node(row, col) {}
AssignNode::AssignNode(const int &row, const int &col, const shared_ptr<Node> &lhs,
             const shared_ptr<Node> &rhs)
      : Node(row, col), lhs_(lhs), rhs_(rhs) {}
      AssignNode::~AssignNode() {}
  void AssignNode::setLHS(const shared_ptr<Node> &lhs) { lhs_ = lhs; }
  void AssignNode::setRHS(const shared_ptr<Node> &rhs) { rhs_ = rhs; }
  shared_ptr<Node> AssignNode::getLHS() const { return lhs_; }
  shared_ptr<Node> AssignNode::getRHS() const { return rhs_; }
  string AssignNode::getHead() const { return head_; }


  string AssignNode::head_ = "Assign";
