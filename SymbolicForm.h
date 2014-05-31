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
#include    "rapidxml/rapidxml.h"

using namespace std::string;
using namespace std::is_integral;
using namespace std::true_type;
using namespace std::stringstream;

template <>
static inline string toString() {
    return "";
}

template <typename T, typename Ts...>
static inline string toString(const T& first, const Ts&... rest) {
    stringstream ss;
    if (is_integral<T>::value) {
        if (first == true_type) {
            ss << "True";
        } else {
            ss << "False";
        }
    } else {
        ss << first;
    }
    if (sizeof...(rest) == 0) {
        return ss.str();
    }
    ss << toString(rest);
    return ss;
}

class SymbolicAtom;
class SymbolicStatement;

sturct SymbolicFormBase {
public:
	~SBase() {}
	virtual inline json_t toJSON() = 0;
	virtual inline string toMLink() = 0;
	virtual inline string toCString() = 0;
	virtual inline bool isAtom() { return false; }
	virtual inline bool isStatement() { return false; }
};



#endif /* __SYMBOLIC_FORM_H__ */




#endif
