#include <pw/impl/swap.h>

#include <catch2/catch.hpp>

SCENARIO("validate swap()", "[swap, algorithm]")
{
    GIVEN("Two integers")
    {
        int const v1   = 3;
        int const v2   = 5;
        int       val1 = v1;
        int       val2 = v2;
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
