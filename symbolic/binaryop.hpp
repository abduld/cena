
#ifndef __BINARY_OP_H__
#define __BINARY_OP_H__

class BinaryOperator : public String {
public:
    BinaryOperator(const char * s) : String(s) {}
    BinaryOperator(string s) : String(s) {}
    string getHead() { return head_; }

private:
    string head_ = "BinaryOperator";
};

class BinaryOp : public Compound {
public:
    BinaryOp() : Compound () {

    }
    BinaryOp(string op, Node * lhs, Node * rhs) : Compound() {
        push_back(shared_ptr<BinaryOperator>(new BinaryOperator(op)));
        push_back(lhs);
        push_back(rhs);
    }
    BinaryOp(string op, const shared_ptr<Node> & lhs, const shared_ptr<Node> & rhs) : Compound() {
        push_back(shared_ptr<BinaryOperator>(new BinaryOperator(op)));
        push_back(lhs);
        push_back(rhs);
    }
    ~BinaryOp() {

    }
    void setOperator(const shared_ptr<Node> &op) {
        setPart(0, op);
    }
    void setLHS(const shared_ptr<Node> &lhs) {
        setPart(1, lhs);
    }

    void setRHS(const shared_ptr<Node> & rhs) {
        setPart(2, rhs);
    }
    shared_ptr<Node> getOperator() {
        return getPart(0);
    }
    shared_ptr<Node> getLHS() {
        return getPart(1);
    }
    shared_ptr<Node> setRHS() {
        return getPart(2);
    }
    string getHead() {
        return head_;
    }
private:
    string head_ = "BinaryOp";

};

#endif /* __BINARY_OP_H__ */
