#include "test_values.h"

#include <test_testtype.h>

#include <catch2/catch.hpp>

/*
 * Type requirements:
 * - No extra
 */
TEMPLATE_LIST_TEST_CASE("iterator methods on a vector", "[vector][iter]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        REQUIRE(pw::is_same<value_type*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<value_type, typename Vector::value_type>::value);
        WHEN("begin() is called")
        {
            typename Vector::iterator iter;
            iter = v.begin();
            THEN("cbegin() is same")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
            THEN("cend() is same as end()")
            {
                REQUIRE(iter == v.cend());
            }
        }
        WHEN("rbegin() is called")
        {
            typename Vector::reverse_iterator riter;
            riter = v.rbegin();
            THEN("crbegin() is same")
            {
                REQUIRE(riter == v.crbegin());
            }
            THEN("rend() is same")
            {
                REQUIRE(riter == v.rend());
            }
            THEN("crend() is same as rend()")
            {
                REQUIRE(riter == v.crend());
            }
        }
    }
    GIVEN("An empty const vector")
    {
        Vector const v;
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);

        WHEN("begin() is called")
        {
            typename Vector::const_iterator iter;
            iter = v.begin();
            THEN("cend() is same")
            {
                REQUIRE(iter == v.cend());
            }
            THEN("cbegin() is same")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
            THEN("cend() is same as end()")
            {
                REQUIRE(iter == v.cend());
            }
        }
    }
    GIVEN("A vector with 5 TestType")
    {
        pw::test::Values<Vector> gen { 5 };
        Vector&                  v = gen.values;
        WHEN("begin() is called")
        {
            typename Vector::iterator iter;
            iter = v.begin();
            THEN("cbegin() is same")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("iter points to first value")
            {
                REQUIRE(*iter == gen.first_value);
            }
        }
        WHEN("rbegin() is called")
        {
            typename Vector::reverse_iterator iter;
            iter = v.rbegin();
            THEN("crbegin() is same")
            {
                REQUIRE(iter == v.crbegin());
            }
            THEN("*iter is last value")
            {
                REQUIRE(*iter == gen.last_value);
            }
            THEN("end() is same")
            {
                REQUIRE(iter.base() == v.end());
            }
        }
        WHEN("rend() is called")
        {
            typename Vector::reverse_iterator iter;
            iter = v.rend();
            THEN("crend() is same")
            {
                REQUIRE(iter == v.crend());
            }
            THEN("iter.base() is begin()")
            {
                REQUIRE(iter.base() == v.begin());
            }
            THEN("iter is first value")
            {
                REQUIRE(*(iter - 1) == gen.first_value);
            }
        }
    }
    GIVEN("An empty const vector")
    {
        Vector const v;
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);

        WHEN("begin() is called")
        {
            typename Vector::const_iterator iter;
            iter = v.begin();
            THEN("cend() is same")
            {
                REQUIRE(iter == v.cend());
            }
            THEN("cbegin() is same")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
            THEN("cend() is same as end()")
            {
                REQUIRE(iter == v.cend());
            }
        }
    }
}
