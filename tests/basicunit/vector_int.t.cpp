#include "basicunit_allocator.h"

#include <pw/vector>

#include <catch2/catch.hpp>

/*
 * Type Requirements:
 * - No extra
 */
TEST_CASE("Constructors", "[vector][constructor]")
{
    using Vector     = pw::vector<int>;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector v;
        WHEN("Nothing changes")
        {
            THEN("empty() is true") { REQUIRE(v.empty()); }
            THEN("size() is 0") { REQUIRE(0 == v.size()); }
            THEN("begin() is null") { REQUIRE(!v.begin()); }
            THEN("end() is null") { REQUIRE(!v.end()); }
        }
    }
    GIVEN("A vector of count values")
    {
        pw::size_t const total = 10;
        value_type const value = 3;
        Vector           v(total, value);

        WHEN("Initialized with value")
        {
            THEN("empty() is false") { REQUIRE(!v.empty()); }
            THEN("size() is total") { REQUIRE(total == v.size()); }
            THEN("begin() returns element")
            {
                REQUIRE(v.begin());
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(v.end());
                REQUIRE(*(v.end() - 1) == value);
            }
        }
    }
}

TEST_CASE("Constructors with Allocator", "[vector][constructor][allocator]")
{
    using Allocator  = basicunit::my_allocator<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector with allocator")
    {
        Allocator allocator;
        Vector    v(allocator);
        WHEN("Check allocator")
        {
            THEN("allocator is same") { REQUIRE(allocator == v.get_allocator()); }
        }
    }
    GIVEN("Two empty vectors with allocator")
    {
        Vector v1;
        Vector v2;
        WHEN("Default allocator")
        {
            THEN("allocators are different") { REQUIRE(!(v1.get_allocator() == v2.get_allocator())); }
        }
    }
}
