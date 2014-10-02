

#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "ast.hpp"
class Statement : public Compound {
public:
    Statement() : Compound () {

    }
    ~Statement() {

    }
    bool  isStatement() const {
        return true;
    }
    string getHead() {
        return head_;
    }
private:
    string head_ = "Statement";

};

#endif /* __STATEMENT_H__ */

