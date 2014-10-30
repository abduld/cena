
#ifndef __NODE_DISTANCE_H__
#define __NODE_DISTANCE_H__

#include <type_traits>
#include "levenshtein.hpp"


template <typename T>
int node_distance(const T & s, const T & t) {
    return 0;
}

template <>
int node_distance<IntegerNode>(const IntegerNode & s, const IntegerNode & t) {
    return 0;
}

template <>
int node_distance<RealNode>(const RealNode & s, const RealNode & t) {
    return 0;
}

template <>
int node_distance<StringNode>(const StringNode & s, const StringNode & t) {
    return levenshtein_distance(s.getConstant(), t.getConstant());
}

template <>
int node_distance<SymbolNode>(const SymbolNode & s, const SymbolNode & t) {
    return levenshtein_distance(s.getConstant(), t.getConstant());
}

template <>
int node_distance<IdentifierNode>(const IdentifierNode & s, const IdentifierNode & t) {
    return levenshtein_distance(s.getName(), t.getName());
}

template <typename T1, typename T2>
std::enable_if<std::is_same<T1, T2>::value, int>
node_distance(const T1 & s, const T2 & t) {
    return node_distance<T>(s, t);
}


#endif /* __NODE_DISTANCE_H__ */

