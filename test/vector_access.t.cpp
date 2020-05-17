#include <pw/type_traits>
#include <pw/vector>
#include <test_testtype.h>

#include <catch2/catch.hpp>

#include <stdexcept>

TEMPLATE_LIST_TEST_CASE("access methods", "[vector][access]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        REQUIRE(pw::is_same<value_type*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<value_type, typename Vector::value_type>::value);

        WHEN("data() is called")
        {
            THEN("it returns NULL")
            {
                REQUIRE(!v.data());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

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
