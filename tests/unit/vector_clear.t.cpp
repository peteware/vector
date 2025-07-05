#include <pw/algorithm>
#include <test_testtype.h>
#include <test_values.h>

#include <catch2/catch.hpp>

/*
 * Type requirements
 * - clear(): No extra
 */
TEMPLATE_LIST_TEST_CASE("clear methods",
                        "[vector][clear]",
                        pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("clear() is called")
        {
            v.clear();
            THEN("nothing goes wrong")
            {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(),
                          generate.values.end(),
                          v.begin(),
                          v.end()));

        WHEN("clear() is called")
        {
            v.clear();
            THEN("it is empty")
            {
                REQUIRE(v.empty());
            }
            THEN("capacity() is same")
            {
                REQUIRE(generate.count == v.capacity());
            }
        }
        WHEN("clear() is called then shrink_to_fit()")
        {
            v.clear();
            v.shrink_to_fit();
            THEN("capacity() is 0")
            {
                REQUIRE(0 == v.capacity());
            }
        }
    }
}
