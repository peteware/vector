#include "catch2/catch.hpp"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("at", "[vector][]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;

        WHEN("at() is called")
        {
            THEN("at(0) fails")
            {
                CHECK_THROWS_AS(v.at(0), std::out_of_range);
            }
            THEN("at(10) fails")
            {
                CHECK_THROWS_AS(v.at(10), std::out_of_range);
            }
        }
    }
    GIVEN("A const empty vector")
    {
        Vector const v;

        WHEN("at() const is called")
        {
            THEN("at(0) const fails")
            {
                CHECK_THROWS_AS(v.at(0), std::out_of_range);
            }
            THEN("at(10) const fails")
            {
                CHECK_THROWS_AS(v.at(10), std::out_of_range);
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        Values<Vector> generate(5);
        Vector         v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));
        WHEN("at(0) is called")
        {
            value_type& r = v.at(0);
            THEN("it works")
            {
                REQUIRE(generate.first_value == r);
            }
        }
        WHEN("at(count) is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(v.at(generate.count), std::out_of_range);
            }
        }
    }
    GIVEN("A const vector of value_type with 1 item")
    {
        Vector        v;
        Vector const& c = v;

        v.push_back(value_type());
        WHEN("at(0) const is called")
        {
            value_type const& r = c.at(0);
            THEN("it works")
            {
                REQUIRE(r == value_type());
            }
        }
        WHEN("at(1) const is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(c.at(1), std::out_of_range);
            }
        }
    }
}
