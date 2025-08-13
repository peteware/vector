#include <test_testtype.h>
#include <test_values.h>

#include <catch2/catch_all.hpp>

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
            auto iter = v.begin();
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
        }
        WHEN("cbegin() is called")
        {
            auto iter = v.cbegin();
            THEN("cend() is same")
            {
                REQUIRE(iter == v.cend());
            }
        }
        WHEN("rbegin() is called")
        {
            auto iter = v.rbegin();
            THEN("rend() is same")
            {
                REQUIRE(iter == v.rend());
            }
            THEN("crend() is same as rend()")
            {
                REQUIRE(iter == v.crend());
            }
        }
        WHEN("crbegin() is called")
        {
            auto iter = v.crbegin();
            THEN("rend() is same")
            {
                REQUIRE(iter == v.rend());
            }
            THEN("crend() is same")
            {
                REQUIRE(iter == v.crend());
            }
        }
    }
    GIVEN("An empty const vector")
    {
        Vector const v;
        WHEN("begin() is called")
        {
            auto iter = v.begin();
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
        }
        WHEN("cbegin() is called")
        {
            auto iter = v.cbegin();
            THEN("cend() is same")
            {
                REQUIRE(iter == v.cend());
            }
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
        }
        WHEN("rbegin() is called")
        {
            auto iter = v.rbegin();
            THEN("rend() is same")
            {
                REQUIRE(iter == v.rend());
            }
            THEN("crend() is same")
            {
                REQUIRE(iter == v.crend());
            }
        }
        WHEN("crbegin() is called")
        {
            auto iter = v.crbegin();
            THEN("crend() is same")
            {
                REQUIRE(iter == v.crend());
            }
        }
    }
    GIVEN("A vector with 5 TestType")
    {
        pw::test::Values<Vector> gen { 5 };
        Vector&                  v = gen.values;
        WHEN("begin() is called")
        {
            auto iter = v.begin();
            THEN("begin() is same as cbegin()")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("iter points to first value")
            {
                REQUIRE(*iter == gen.first_value);
            }
        }
        WHEN("cbegin() is called")
        {
            auto iter = v.cbegin();
            THEN("begin() is same")
            {
                REQUIRE(iter == v.begin());
            }
            THEN("iter points to first value")
            {
                REQUIRE(*iter == gen.first_value);
            }
        }
        WHEN("rbegin() is called")
        {
            auto iter = v.rbegin();
            THEN("crbegin() is same")
            {
                REQUIRE(iter == v.crbegin());
            }
            THEN("*iter is last value")
            {
                REQUIRE(*iter == gen.last_value);
            }
            THEN("iter.base() is end()")
            {
                REQUIRE(iter.base() == v.end());
            }
        }
        WHEN("crbegin() is called")
        {
            auto iter = v.crbegin();
            THEN("iter.base() is cend()")
            {
                REQUIRE(iter.base() == v.cend());
            }
            THEN("*iter is last value")
            {
                REQUIRE(*iter == gen.last_value);
            }
        }
        WHEN("end() is called")
        {
            auto iter = v.end();
            THEN("iter points to last value")
            {
                REQUIRE(*(iter - 1) == gen.last_value);
            }
        }
        WHEN("cend() is called")
        {
            auto iter = v.cend();
            THEN("iter points to last value")
            {
                REQUIRE(*(iter - 1) == gen.last_value);
            }
        }
        WHEN("rend() is called")
        {
            auto iter = v.rend();
            THEN("iter.base() points to begin()")
            {
                REQUIRE(iter.base() == v.begin());
            }
            THEN("iter - 1 points to first value")
            {
                REQUIRE(*(iter - 1) == gen.first_value);
            }
        }
        WHEN("crend() is called")
        {
            auto iter = v.crend();
            THEN("iter.base() points to begin()")
            {
                REQUIRE(iter.base() == v.begin());
            }
            THEN("iter - 1 points to first value")
            {
                REQUIRE(*(iter - 1) == gen.first_value);
            }
        }
    }
    GIVEN("An const vector with 3 elements")
    {
        pw::test::Values<Vector> gen(3);
        Vector const             v { gen.values[0], gen.values[1], gen.values[2] };
        WHEN("begin() is called")
        {
            auto iter = v.begin();
            THEN("cbegin() is same")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("iter points to first element")
            {
                REQUIRE(*iter == gen.first_value);
            }
        }
        WHEN("cbegin() is called")
        {
            auto iter = v.cbegin();
        }
        WHEN("rbegin() is called")
        {
            auto iter = v.rbegin();

            THEN("iter is crbegin()")
            {
                REQUIRE(iter == v.crbegin());
            }
            THEN("iter points to last element")
            {
                REQUIRE(*iter == gen.last_value);
            }
            THEN("iter.base() is same as end()")
            {
                REQUIRE(iter.base() == v.end());
            }
        }
        WHEN("crbegin() is called")
        {
            auto iter = v.crbegin();
            THEN("rbegin() is same")
            {
                REQUIRE(iter == v.rbegin());
            }
            THEN("iter points to last value")
            {
                REQUIRE(*iter == gen.last_value);
            }
        }
        WHEN("end() is called")
        {
            auto iter = v.end();
            THEN("iter points to last value")
            {
                REQUIRE(*(iter - 1) == gen.last_value);
            }
            THEN("cend() is same")
            {
                REQUIRE(iter == v.cend());
            }
        }
        WHEN("cend() is called")
        {
            auto iter = v.cend();
            THEN("iter points to last value")
            {
                REQUIRE(*(iter - 1) == gen.last_value);
            }
        }
        WHEN("rend() is called")
        {
            auto iter = v.rend();
            THEN("crend() is same")
            {
                REQUIRE(iter == v.crend());
            }
            THEN("iter.base() points to begin()")
            {
                REQUIRE(iter.base() == v.begin());
            }
            THEN("iter - 1 points to first value")
            {
                REQUIRE(*(iter - 1) == gen.first_value);
            }
        }
        WHEN("crend() is called")
        {
            auto iter = v.crend();
            THEN("iter.base() points to begin()")
            {
                REQUIRE(iter.base() == v.begin());
            }
            THEN("iter - 1 points to first value")
            {
                REQUIRE(*(iter - 1) == gen.first_value);
            }
        }
    }
}
