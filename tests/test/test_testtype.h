#ifndef INCLUDED_PW_TEST_TESTTYPE_H
#define INCLUDED_PW_TEST_TESTTYPE_H

#include <pw/vector>
#include <test_allocator_base.h>
#include <test_permute.h>

#include <string>
#include <tuple>
#include <vector>

namespace pw::test {
// I use this type list to make sure tests work with std container
//using TestTypeList = std::tuple<std::vector<int>>;

// I use this for testing a type that can take an allocator
using TestTypeList = std::tuple<pw::vector<int>, pw::vector<std::string>, std::vector<int>>;

// This just for pw::vector implementation
using TestTypeListInt = std::tuple<pw::vector<int>, std::vector<int>>;

// Phase 1 test type list - using int with pw::vector and std::vector for comparison
using Phase1TestTypeList = std::tuple<pw::vector<int>, std::vector<int>>;
// Phase 2 test type list - Adds test::allocator_base to track its usage
using Phase2TestTypeList =
    std::tuple<pw::vector<int, allocator_base<int>>, std::vector<int, allocator_base<int>>>;
// vector<int> but using allocator_base
using TestTypeListAllocatorBase =
    std::tuple<pw::vector<int, allocator_base<int>>, std::vector<int, allocator_base<int>>>;
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_TESTTYPE_H */
