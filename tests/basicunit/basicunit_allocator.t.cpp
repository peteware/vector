#include "basicunit_allocator.h"

#include <catch2/catch.hpp>

TEST_CASE("TestAllocators", "[test][allocator]")
{
    using Allocator  = basicunit::allocator_copy_assignment<int>;
    using value_type = typename Allocator::value_type;

    GIVEN("An Allocator")
    {
        Allocator   alloc;
        value_type* ptr = alloc.allocate(1);
        REQUIRE(ptr);
        alloc.deallocate(ptr, 1);
    }
}
