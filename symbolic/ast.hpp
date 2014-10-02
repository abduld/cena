
#ifndef __AST_H__
#define __AST_H__

#include "gtest/gtest.h"
#include <memory>
#include <iostream>
#include <cassert>
#include <iterator>
#include <string>
#include <vector>
#include "stdio.h"
#include "stdlib.h"

using std::shared_ptr;
using std::string;
using std::vector;

#define DEBUG printf("DEBUG :: >>> %s %d ... \n", __PRETTY_FUNCTION__, __LINE__)

#include "node.hpp"
#include "atom.hpp"
#include "compound.hpp"

#endif /* __AST_H__ */
