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
#include    <cstdint>
#include    "rapidjson/rapidjson.h"
#include    "rapidxml/rapidxml.hpp"

using std::is_integral;
using std::true_type;
using std::string;
using std::vector;
using std::is_same;
using std::stringstream;
using rapidjson::StringStream;


inline void build_string (std::ostream& o) { }
template<class First, class... Rest> inline void build_string (std::ostream& o, const First& first, const Rest&... rest)
{
    if (is_same<First, bool>::value) {
        if (first == true_type::value) {
            o << "True";
        } else {
            o << "False";
        }
    } else {
        o << first;
    }
    build_string(o, rest...);
}
template<class... T> std::string concat_string (const T&... value)
{
    std::ostringstream      o;
    build_string(o, value...);
    return o.str();
}

static inline string toString() {
    return "";
}

template <typename T, typename... Ts>
static inline string toString(const T& first, const Ts&... rest) {
    return concat_string(first, rest...);
}


class MInteger64 {
private:
    int64_t val;
    static constexpr char _head[] = "Integer";
public:
    MInteger64(const int64_t _val) : val(_val) {}
};

class Expr {
    
	~SymbolicFormBase() {}
    virtual inline string getHead() = 0;
	virtual inline StringStream toJSON() = 0;
	virtual inline string toMLink() = 0;
	virtual inline string toCString() = 0;
	virtual inline bool isAtom() { return false; }
	virtual inline bool isStatement() { return false; }
};

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



