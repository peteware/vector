#include <pw/impl/algorithm/copy.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("copy", "[copy]")
{
    GIVEN("An array of int")
    {
        int val[5] = { 0, 1, 2, 3, 4 };
        int dst[5] = { 5, 4, 3, 2, 1 };

        WHEN("val is copied")
        {
            pw::copy(&val[0], &val[5], &dst[0]);
            THEN("copy succeeded")
            {
                REQUIRE(val[0] == dst[0]);
                REQUIRE(val[4] == dst[4]);
            }
        }
    }
}
