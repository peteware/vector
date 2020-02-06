#include "catch2/catch.hpp"
#include <pw/internal/allocate.h>

using TestTypeList = std::tuple<int, double>;

TEMPLATE_LIST_TEST_CASE("check impl/allocate", "[allocate][template]", TestTypeList)
{
    using Allocator = pw::allocator<TestType>;
    using Allocate  = pw::internal::Allocate<TestType, Allocator>;

    Allocator allocator;

    GIVEN("An empty Allocate")
    {
        Allocate allocate(allocator);
        THEN("the size is 0")
        {
            REQUIRE((pw::size_t)0 == allocate.size());
            REQUIRE((pw::size_t)0 == allocate.capacity());
        }
        THEN("hasroom() is false")
        {
            CHECK_FALSE(allocate.hasroom(1));
        }
        THEN("allocate(10) allocates at least that space")
        {
            allocate.allocate(10);
            REQUIRE((pw::size_t)0 == allocate.size());
            REQUIRE((pw::size_t)10 == allocate.capacity());
        }
    }

    GIVEN("An Allocate with capacity 1")
    {
        Allocate allocate(allocator);
        allocate.allocate(1);

        THEN("size() is 0")
        {
            REQUIRE((pw::size_t)0 == allocate.size());
        }
        THEN("capacity() is 1")
        {
            REQUIRE((pw::size_t)1 == allocate.capacity());
        }
        THEN("hasroom(1) is true")
        {
            REQUIRE(allocate.hasroom(1));
        }
        THEN("hasroom(2) is false")
        {
            REQUIRE_FALSE(allocate.hasroom(2));
        }
    }
}
