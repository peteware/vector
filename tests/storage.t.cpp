#include "catch2/catch.hpp"
#include "permute.t.h"
#include <pw/internal/storage.h>

//using TestTypeList = std::tuple<int, double>;
using TestTypeList = std::tuple<int>;

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
        WHEN("Storage(move, 20)")
        {
            Storage s = Storage(pw::move(storage), 20);
            THEN("size() is 0 but capacity increased")
            {
                REQUIRE((pw::size_t)0 == s.size());
                REQUIRE((pw::size_t)20 == s.capacity());
            }
            THEN("storage is empty")
            {
                REQUIRE(0 == storage.size());
                REQUIRE((pw::size_t)10 == storage.capacity());
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
    GIVEN("An allocated Storage with 10")
    {
        Storage  storage(10, allocator);
        TestType value;
        pw::internal::permute(value, 3);
        WHEN("push_back(value)")
        {
            storage.push_back(value);
            THEN("size() == 1")
            {
                REQUIRE(1 == storage.size());
            }
            THEN("capacity() == 10")
            {
                REQUIRE(10 == storage.capacity());
            }
            THEN("*begin() == value")
            {
                REQUIRE(value == *storage.begin());
            }
        }
    }
    GIVEN("An allocated Storage with 10 and one element")
    {
        Storage  storage(10, allocator);
        TestType value;
        pw::internal::permute(value, 3);
        storage.push_back(value);
        REQUIRE(*storage.begin() == value);
        WHEN("Storage (move, 20)")
        {
            Storage s = Storage(pw::move(storage), 20);
            THEN("size() is 1")
            {
                REQUIRE(1 == s.size());
                REQUIRE(20 == s.capacity());
                REQUIRE(1 == storage.size());
                REQUIRE(10 == storage.capacity());
            }
            THEN("element is moved")
            {
                REQUIRE(*s.begin() == value);
            }
        }
        WHEN("operator=() is called")
        {
            Storage s(10, allocator);
            s.push_back(value);
            s = storage;
            THEN("they are same and destruction works")
            {
                REQUIRE(s.size() == storage.size());
                REQUIRE(s.capacity() == s.size());
            }
        }
        WHEN("move() is called")
        {
            storage.move(0, 3, value);
            THEN("3 new elements")
            {
                REQUIRE(4 == storage.size());
            }
        }
    }
}
