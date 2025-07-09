#ifndef INCLUDED_PW_TEST_TESTTYPE_H
#define INCLUDED_PW_TEST_TESTTYPE_H

#include <pw/vector>
#include <string>
#include <tuple>
#include <vector>

#include <test_permute.h>

namespace pw::test {
using TestTypeList = std::tuple<pw::vector<int>, pw::vector<std::string>, std::vector<int>>;
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_TESTTYPE_H */
