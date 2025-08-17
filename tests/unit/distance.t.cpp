#include <pw/impl/distance.h>

#include <test_random_access_iterator.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <pw/internal/rsize_fix.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("validate distance() algorithm", "[distance][algorithm]")
{
    GIVEN("Two pointers")
    {
        constexpr char str[] = "abcdef";
        char const*    s     = &str[0];
        char const*    e     = &str[4];
        WHEN("get the distance")
        {
            const pw::ptrdiff_t d = pw::distance(s, e);
            THEN("distance is 4")
            {
                REQUIRE(4 == d);
            }
        }
        WHEN("difference is negative")
        {
            pw::ptrdiff_t const d = pw::distance(e, s);
            THEN("distance is -4")
            {
                REQUIRE(-4 == d);
            }
        }
        WHEN("nullptr are used")
        {
            char*               a = nullptr;
            pw::ptrdiff_t const d = pw::distance(a, a);
            THEN("distances is 0")
            {
                REQUIRE(0 == d);
            }
        }
    }
}
