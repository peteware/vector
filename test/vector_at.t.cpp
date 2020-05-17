#include <catch2/catch.hpp>
#include <pw/algorithm>
#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("at", "[vector][at]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));
    }
    GIVEN("A const vector of value_type with 1 item")
    {
        pw::test::Values<Vector> generate(1);
        Vector const&            c = generate.values;
    }
}
