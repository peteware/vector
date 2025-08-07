#include <pw/impl/distance.h>

#include <test/test_random_access_iterator.h>

#include <catch2/catch.hpp>

SCENARIO("validate distance() algorithm", "[distance][algorithm]")
{
    GIVEN("Two pointers")
    {
        char const  str[] = "abcdef";
        char const* s     = &str[0];
        char const* e     = &str[4];
        WHEN("get the distance")
        {
            pw::ptrdiff_t d = pw::distance(s, e);
            THEN("distance is 4")
            {
                REQUIRE(4 == d);
            }
        }
        WHEN("difference is negative")
        {
            pw::ptrdiff_t d = pw::distance(e, s);
            THEN("distance is -4")
            {
                REQUIRE(-4 == d);
            }
        }
        WHEN("nullptrs are used")
        {
            char*         a = nullptr;
            pw::ptrdiff_t d = pw::distance(a, a);
            THEN("distances is 0")
            {
                REQUIRE(0 == d);
            }
        }
    }
}
