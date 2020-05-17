#include <catch2/catch.hpp>
#include <pw/algorithm>
#include <test_permute.h>
//#include <test_same.h>
#include <test_testtype.h>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

#include <stdexcept>

TEMPLATE_LIST_TEST_CASE("clear methods", "[vector][clear]", pw::test::TestTypeList)
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
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

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
