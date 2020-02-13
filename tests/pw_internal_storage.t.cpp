#include "catch2/catch.hpp"
#include "pw_permute.t.h"
#include <pw/internal/storage.h>

using TestTypeList = std::tuple<int, double>;

TEMPLATE_LIST_TEST_CASE("check impl/storage", "[storage]", TestTypeList)
{
    using Allocator = pw::allocator<TestType>;
    using Storage   = pw::internal::Storage<TestType, Allocator>;
    Allocator allocator;

    GIVEN("An empty Storage")
    {
        Storage storage(allocator);
        WHEN("nothing is changed")
        {
            THEN("it's all empty")
            {
                REQUIRE((pw::size_t)0 == storage.size());
                REQUIRE((pw::size_t)0 == storage.capacity());
                REQUIRE(storage.begin() == storage.end());
            }
            THEN("newsize() is larger")
            {
                REQUIRE(storage.newsize() > 0);
            }
        }
    }
    GIVEN("An allocated Storage with 10")
    {
        Storage storage(10, allocator);
        WHEN("move(20) is called")
        {
            Storage s = storage.move(20);
            THEN("size() is 0 but capacity increased")
            {
                REQUIRE((pw::size_t)0 == s.size());
                REQUIRE((pw::size_t)20 == s.capacity());
            }
        }
        WHEN("newsize() is called")
        {
            THEN("newsize() is larger")
            {
                REQUIRE(storage.newsize() > 10);
            }
        }
    }
    GIVEN("An allocated Storage with 10 and one element")
    {
        Storage  storage(10, allocator);
        TestType value;
        pw::internal::permute(value, 3);
        storage.push_back(value);
        WHEN("move(20) is called")
        {
            Storage s = storage.move(20);
            THEN("size() is 1")
            {
                REQUIRE(1 == s.size());
                REQUIRE(0 == storage.size());
                REQUIRE(10 == storage.capacity());
            }
            THEN("element is moved")
            {
                REQUIRE(*s.begin() == value);
            }
        }
    }
}
