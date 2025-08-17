#include <pw/impl/swap.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <pw/internal/rsize_fix.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("validate swap()", "[swap, algorithm]")
{
    GIVEN("Two integers")
    {
        constexpr int v1   = 3;
        constexpr int v2   = 5;
        int           val1 = v1;
        int           val2 = v2;
        WHEN("swap() them")
        {
            pw::swap(val1, val2);
            THEN("they swapped")
            {
                REQUIRE(val1 == v2);
                REQUIRE(val2 == v1);
            }
        }
    }
}
