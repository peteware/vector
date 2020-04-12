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
        size_t const count = 5;
        value_type   first_value;
        value_type   last_value;
        value_type   value;
        Vector       values;

        for (size_t i = 0; i < count; ++i)
        {
            permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
            last_value = value;
        }
        Vector v(values);
        REQUIRE(pw::equal(values.begin(), values.end(), v.begin(), v.end()));
        WHEN("at(0) is called")
        {
            value_type& r = v.at(0);
            THEN("it works")
            {
                REQUIRE(first_value == r);
            }
        }
        WHEN("at(count) is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(v.at(count), std::out_of_range);
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
