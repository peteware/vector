#ifndef INCLUDED_PW_TEST_TESTTYPE_H
#define INCLUDED_PW_TEST_TESTTYPE_H

#include "test_allocator_first_type.h"
#include "test_allocator_last_type.h"
#include "test_allocator_only_type.h"
#include "test_no_allocator_type.h"

#include <pw/vector>
#include <test_allocator_base.h>
#include <test_permute.h>
#include <test_throwing_allocator.h>
#include <test_throwingtype.h>

#include <string>
#include <tuple>
#include <vector>

namespace pw::test {
// I use this type list to make sure tests work with std container
//using TestTypeList = std::tuple<std::vector<int>>;

// I use this for testing a type that can take an allocator
using TestTypeList = std::tuple<vector<int>, vector<std::string>, std::vector<int>>;

// This just for pw::vector implementation
using TestTypeListInt = std::tuple<vector<int>, std::vector<int>>;

// Phase 1 test type list - using int with pw::vector and std::vector for comparison
using Phase1TestTypeList = std::tuple<vector<int>, std::vector<int>>;
// Phase 2 test type list - Adds test::allocator_base to track its usage
using Phase2TestTypeList =
    std::tuple<vector<int, allocator_base<int>>, std::vector<int, allocator_base<int>>>;
// vector<int> but using allocator_base
using TestTypeListAllocatorBase =
    std::tuple<vector<int, allocator_base<int>>, std::vector<int, allocator_base<int>>>;

using TestTypeListThrowing = std::tuple<vector<ThrowingType, ThrowingAllocator<ThrowingType>>,
                                        std::vector<ThrowingType, ThrowingAllocator<ThrowingType>>>;
using TestTypeListNoAllocator =
    std::tuple<vector<pw::test::NoAllocatorType>, std::vector<pw::test::NoAllocatorType>>;
using TestTypeListAllocatorOnly  = std::tuple<vector<pw::test::AllocatorOnlyType<allocator<int>>>,
                                              std::vector<pw::test::AllocatorOnlyType<allocator<int>>>>;
using TestTypeListAllocatorFirst = std::tuple<vector<pw::test::AllocatorFirstType<allocator<int>>>,
                                              std::vector<pw::test::AllocatorFirstType<allocator<int>>>>;
using TestTypeListAllocatorLast  = std::tuple<vector<pw::test::AllocatorLastType<allocator<int>>>,
                                              std::vector<pw::test::AllocatorLastType<allocator<int>>>>;

template<typename Type>
void
reset(Type& value)
{
    value.reset();
}
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_TESTTYPE_H */
