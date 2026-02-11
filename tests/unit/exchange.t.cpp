#include <pw/impl/utility/exchange.h>

#include <string>

// ReSharper disable once CppUnusedIncludeDirective
#include <pw/internal/rsize_fix.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("exchange", "[utility]")
{
    GIVEN("An int")
    {
        int val = 2;
        WHEN("exchange(val, 3)")
        {
            int n = pw::exchange(val, 4);
            THEN("they exchanged values")
            {
                REQUIRE(n == 2);
                REQUIRE(val == 4);
            }
        }
    }
    GIVEN("A std::string")
    {
        std::string val("abc");
        WHEN("exchange(val, 'def'")
        {
            std::string n = pw::exchange(val, "def");
            THEN("they exchanged values")
            {
                REQUIRE(n == "abc");
                REQUIRE(val == "def");
            }
        }
    }
}
