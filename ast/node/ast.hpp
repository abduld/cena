
#ifndef __AST_H__
#define __AST_H__

#include "gtest/gtest.h"
#include <memory>
#include <cinttypes>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include "stdio.h"
#include "stdlib.h"
#include "json11/json11.hpp"

using namespace json11;
using std::int64_t;
using std::shared_ptr;
using std::string;
using std::vector;
using std::ostringstream;
using std::transform;

#define DEBUG printf("DEBUG :: >>> %s %d ... \n", __PRETTY_FUNCTION__, __LINE__)

#include "node.hpp"
#include "utilities.hpp"
#include "atom.hpp"
#include "identifier.hpp"
#include "type.hpp"
#include "keyword.hpp"
#include "compound.hpp"
#include "statement.hpp"
#include "block.hpp"
#include "assign.hpp"
#include "binaryop.hpp"
#include "unaryop.hpp"
#include "call.hpp"
#include "break.hpp"
#include "cudalaunch.hpp"
#include "define.hpp"
#include "do.hpp"
#include "false.hpp"
#include "for.hpp"
#include "function.hpp"
#include "if.hpp"
#include "keyword.hpp"
#include "literal.hpp"
#include "pragma.hpp"
#include "program.hpp"
#include "return.hpp"
#include "sizeof.hpp"
#include "switch.hpp"
#include "true.hpp"
#include "typedef.hpp"
#include "while.hpp"
#include "declare.hpp"

#endif /* __AST_H__ */
