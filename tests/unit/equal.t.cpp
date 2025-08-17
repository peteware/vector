#include <pw/impl/equal.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <pw/internal/rsize_fix.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("pw::equal() compares integer arrays", "[equal]")
{
    GIVEN("Two identical arrays of int")
    {
        int a[] = { 1, 2, 3, 4, 5 };
        int b[] = { 1, 2, 3, 4, 5 };
        WHEN("pw::equal is called on the full range")
        {
            bool result = pw::equal(&a[0], &a[5], &b[0], &b[5]);
            THEN("The result is true")
            {
                REQUIRE(result);
            }
        }
    }
    GIVEN("Two arrays of int with different values")
    {
        int a[] = { 1, 2, 3, 4, 5 };
        int b[] = { 1, 2, 0, 4, 5 };
        WHEN("pw::equal is called on the full range")
        {
            bool result = pw::equal(&a[0], &a[5], &b[0], &b[5]);
            THEN("The result is false")
            {
                REQUIRE_FALSE(result);
            }
        }
    }
    GIVEN("Two arrays of different lengths")
    {
        int a[] = { 1, 2, 3, 4, 5 };
        int b[] = { 1, 2, 3, 4 };
        WHEN("pw::equal is called on the full range of a and b")
        {
            bool result = pw::equal(&a[0], &a[5], &b[0], &b[4]);
            THEN("The result is false")
            {
                REQUIRE_FALSE(result);
            }
        }
    }
    GIVEN("Two empty arrays")
    {
        int a[] = {};
        int b[] = {};
        WHEN("pw::equal is called on the empty ranges")
        {
            bool result = pw::equal(&a[0], &a[0], &b[0], &b[0]);
            THEN("The result is true")
            {
                REQUIRE(result);
            }
        }
    }
}
