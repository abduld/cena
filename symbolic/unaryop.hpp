
#ifndef __BINARY_OP_H__
#define __BINARY_OP_H__

class UnaryOperator : public String {
public:
    UnaryOperator(const char * s) : String(s) {}
    UnaryOperator(string s) : String(s) {}
    string getHead() { return head_; }

private:
    string head_ = "UnaryOperator";
};

class UnaryOp : public Compound {
public:
    UnaryOp() : Compound () {

    }
    UnaryOp(string op, Node * arg) : Compound() {
        push_back(shared_ptr<UnaryOperator>(new UnaryOperator(op)));
        push_back(arg);
    }
    UnaryOp(string op, const shared_ptr<Node> & arg) : Compound() {
        push_back(shared_ptr<UnaryOperator>(new UnaryOperator(op)));
        push_back(arg);
    }
    ~UnaryOp() {

    }
    void setOperator(const shared_ptr<Node> &op) {
        setPart(0, op);
    }
    void setArg(const shared_ptr<Node> &arg) {
        setPart(1, arg);
    }

    shared_ptr<Node> getOperator() {
        return getPart(0);
    }
    shared_ptr<Node> getArg() {
        return getPart(1);
    }
    string getHead() {
        return head_;
    }
private:
    string head_ = "UnaryOp";

};

#endif /* __BINARY_OP_H__ */
