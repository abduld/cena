
#pragma once


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
#include <boost/variant.hpp>
#include    "rapidjson/rapidjson.h"
#include    "rapidxml/rapidxml.hpp"

using std::is_integral;
using std::is_floating_point;
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

template< typename T >
struct MLiteralHead_
{
    static const string _head;
};

template<> string const MLiteralHead_<bool>::_head = "Boolean";
template<> string const MLiteralHead_<int8_t>::_head = "Integer8";
template<> string const MLiteralHead_<uint8_t>::_head = "UnsignedInteger8";
template<> string const MLiteralHead_<int16_t>::_head = "Integer16";
template<> string const MLiteralHead_<uint16_t>::_head = "UnsignedInteger16";
template<> string const MLiteralHead_<int32_t>::_head = "Integer32";
template<> string const MLiteralHead_<uint32_t>::_head = "UnsignedInteger32";
template<> string const MLiteralHead_<int64_t>::_head = "Integer64";
template<> string const MLiteralHead_<float>::_head = "Real32";
template<> string const MLiteralHead_<double>::_head = "Real64";
template<> string const MLiteralHead_<string>::_head = "String";

template <typename T>
struct MLiteral {
private:
    const T val;
public:
    MLiteral(const T _val) : val(_val) {}
    string getHead() const { return MLiteralHead_<T>()._head; }
    T getValue() const { return val; }
    size_t getByteCount() const { return sizeof(T); }
};

template <class T, T val>
struct MLiteralConstant {
    typedef MLiteralConstant<T, val>  type;
    typedef T                         value_type;
    static const T value = val;
};

typedef MLiteralConstant<int64_t, 0>  eZero;
typedef MLiteralConstant<int64_t, 1>  eOne;

typedef MLiteral<bool> MBool;
typedef MLiteral<int8_t> MInteger8;
typedef MLiteral<uint8_t> MUnsignedInteger8;
typedef MLiteral<int16_t> MInteger16;
typedef MLiteral<uint16_t> MUnsignedInteger16;
typedef MLiteral<int32_t> MInteger32;
typedef MLiteral<uint32_t> MUnsignedInteger32;
typedef MLiteral<int64_t> MInteger64;
typedef MLiteral<float> MReal32;
typedef MLiteral<double> MReal64;
typedef MLiteral<string> MString;


typedef MInteger64 MInteger;
typedef MReal32 MReal;

template<> struct is_integral<MBool> : public true_type {};
template<> struct is_integral<MInteger8> : public true_type {};
template<> struct is_integral<MUnsignedInteger8> : public true_type {};
template<> struct is_integral<MInteger16> : public true_type {};
template<> struct is_integral<MUnsignedInteger16> : public true_type {};
template<> struct is_integral<MUnsignedInteger32> : public true_type {};
template<> struct is_integral<MInteger64> : public true_type {};
template<> struct is_integral<MReal32> : public true_type {};
template<> struct is_integral<MReal64> : public true_type {};


template<> struct is_floating_point<MReal32> : public true_type {};
template<> struct is_floating_point<MReal64> : public true_type {};

struct MExpr;
/*
struct MSymbol {
private:
    const string head;
    const vector<MExpr> body;
public:
    MLiteral(const T _val) : val(_val) {}
    string getHead() const { return MLiteralHead_<T>()._head; }
    T getValue() const { return val; }
    size_t getByteCount() const { return sizeof(T); }
};

template <typename T>
struct MExpr {
private:
    const T val;
public:
    MLiteral(const T _val) : val(_val) {}
    string getHead() const { return MLiteralHead_<T>()._head; }
    T getValue() const { return val; }
    size_t getByteCount() const { return sizeof(T); }
};
 */

#endif /* __SYMBOLIC_FORM_H__ */



