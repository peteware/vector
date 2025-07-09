#include <test_testtype.h>
#include <test_values.h>

#include <pw/algorithm>

#include <catch2/catch.hpp>

#include <stdexcept>

/*
 * Type requirements:
 * - at(): no extra
 * - operator[](): no extra
 * - front(): no extra
 * - back(): no extra
 * - data(): no extra
 */
TEMPLATE_LIST_TEST_CASE("access methods", "[vector][access]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;

        WHEN("data() is called")
        {
            THEN("it returns NULL")
            {
                REQUIRE(!v.data());
            }
        }

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
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
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
        WHEN("v[0]")
        {
            value_type& r = v[0];
            THEN("it is first_value")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("v[count - 1]")
        {
            value_type& r = v[generate.count - 1];
            THEN("it is last_value")
            {
                REQUIRE(r == generate.last_value);
            }
        }
        WHEN("front() is called")
        {
            value_type& r = v.front();
            THEN("it works")
            {
                REQUIRE(generate.first_value == r);
            }
        }
        WHEN("back() is called")
        {
            value_type& r = v.back();
            THEN("it works")
            {
                REQUIRE(generate.last_value == r);
            }
        }
        WHEN("data() is called")
        {
            value_type* p = v.data();
            THEN("it works")
            {
                REQUIRE(generate.first_value == *p);
            }
        }
    }
    GIVEN("A const vector of value_type with 1 item")
    {
        pw::test::Values<Vector> generate(1);
        Vector const&            c = generate.values;

        WHEN("at(0) const is called")
        {
            value_type const& r = c.at(0);
            THEN("it works")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("at(1) const is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(c.at(1), std::out_of_range);
            }
        }
        WHEN("v[0]")
        {
            value_type const& r = c[0];
            THEN("it is first_value")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("v[count - 1]")
        {
            value_type const& r = c[generate.count - 1];
            THEN("it is last_value")
            {
                REQUIRE(r == generate.last_value);
            }
        }
        WHEN("front() const is called")
        {
            value_type const& r = c.front();
            THEN("it works")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("back() const is called")
        {
            value_type const& r = c.back();
            THEN("it works")
            {
                REQUIRE(r == generate.last_value);
            }
        }
        WHEN("data() const is called")
        {
            value_type const* p = c.data();
            THEN("it works")
            {
                REQUIRE(*p == generate.first_value);
            }
        }
    }
}
