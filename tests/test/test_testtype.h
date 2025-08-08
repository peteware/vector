#ifndef INCLUDED_PW_TEST_TESTTYPE_H
#define INCLUDED_PW_TEST_TESTTYPE_H

#include <pw/vector>
#include <string>
#include <tuple>
#include <vector>

#include <test_permute.h>

namespace pw::test {
// I use this type list to make sure tests work with std container
//using TestTypeList = std::tuple<std::vector<int>>;

// I use this for testing a type that can take an allocator
using TestTypeList = std::tuple<pw::vector<int>, pw::vector<std::string>, std::vector<int>>;

// This just for pw::vector implementation

using TestTypeListInt = std::tuple<pw::vector<int>, std::vector<int>>;
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_TESTTYPE_H */
