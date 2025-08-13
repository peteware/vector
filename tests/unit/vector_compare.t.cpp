#include <test_testtype.h>

#include <catch2/catch_all.hpp>

/*
 * Type requirements
 */
TEMPLATE_LIST_TEST_CASE("comparison", "[vector][compare]", pw::test::TestTypeListInt)
{
    using Vector = TestType;

    GIVEN("Two empty vectors")
    {
        Vector v1;
        Vector v2;
        REQUIRE(v1 == v2);
        REQUIRE(!(v1 != v2));
        REQUIRE(!(v1 < v2));
        REQUIRE(v1 <= v2);
        REQUIRE(!(v2 > v1));
        REQUIRE(v2 >= v1);
    }
    GIVEN("Two different sized vectors")
    {
        Vector v1 { 1, 2 };
        Vector v2 { 1, 2, 3 };

        REQUIRE(!(v1 == v2));
        REQUIRE(v1 != v2);
        REQUIRE(v1 < v2);
        REQUIRE(v1 <= v2);
        REQUIRE(v2 > v1);
        REQUIRE(v2 >= v1);
    }
    GIVEN("Two same sized vectors with different values")
    {
        Vector v1 { 1, 2, 3 };
        Vector v2 { 1, 5, 6 };

        REQUIRE(!(v1 == v2));
        REQUIRE(v1 != v2);
        REQUIRE(v1 < v2);
        REQUIRE(v1 <= v2);
        REQUIRE(v2 > v1);
        REQUIRE(v2 >= v1);
    }
}