#ifndef INCLUDED_PW_TEST_TESTTYPE_H
#define INCLUDED_PW_TEST_TESTTYPE_H

#include "test_optracker_allocator_first.h"
#include "test_optracker_allocator_last.h"
#include "test_optracker_allocator_none.h"
#include "test_optracker_allocator_only.h"

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
    std::tuple<vector<pw::test::OpTrackerAllocatorNone>, std::vector<pw::test::OpTrackerAllocatorNone>>;
using TestTypeListAllocatorOnly =
    std::tuple<vector<pw::test::OpTrackerAllocatorOnly>, std::vector<pw::test::OpTrackerAllocatorOnly>>;
using TestTypeListAllocatorFirst = std::tuple<pw::vector<pw::test::OpTrackerAllocatorFirst<allocator<int>>>,
                                              std::vector<pw::test::OpTrackerAllocatorFirst<allocator<int>>>>;
using TestTypeListAllocatorLast  = std::tuple<pw::vector<pw::test::OpTrackerAllocatorLast<allocator<int>>>,
                                              std::vector<pw::test::OpTrackerAllocatorLast<allocator<int>>>>;

template<typename Type>
void
reset(Type& value)
{
    value.reset();
}
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_TESTTYPE_H */
