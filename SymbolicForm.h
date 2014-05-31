//
//  SymbolicForm.h
//  Project
//
//  Created by Abdul Dakkak on 5/30/14.
//
//


#ifndef __SYMBOLIC_FORM_H__
#define __SYMBOLIC_FORM_H__

#include	<vector>
#include	<string>
#include	<ostream>
#include	<iostream>
#include	<sstream>
#include	<cstring>
#include    <ios>
#include    <type_traits>
#include    "rapidjson/rapidjson.h"
#include    "rapidxml/rapidxml.hpp"

using std::is_integral;
using std::true_type;
using std::string;
using std::vector;
using std::is_same;
using std::stringstream;
using rapidjson::StringStream;

static inline string toString() {
    return "";
}

template <typename T, typename... Ts>
static inline string toString(const T& first, const Ts&... rest) {
    stringstream ss;
    if (is_same<T, bool>::value) {
        if (first == true_type::value) {
            ss << "True";
        } else {
            ss << "False";
        }
    } else {
        ss << first;
    }
    if (sizeof...(rest) > 0) {
        ss << toString(rest...);
    }
    return ss.str();
}

class SymbolicAtom;
class SymbolicStatement;

class SymbolicFormBase {
protected:
    string head;
public:
	~SymbolicFormBase() {}
    virtual inline string getHead() = 0;
	virtual inline StringStream toJSON() = 0;
	virtual inline string toMLink() = 0;
	virtual inline string toCString() = 0;
	virtual inline bool isAtom() { return false; }
	virtual inline bool isStatement() { return false; }
};

struct SymbolicReturn : public SymbolicFormBase {
    
};



#endif /* __SYMBOLIC_FORM_H__ */



