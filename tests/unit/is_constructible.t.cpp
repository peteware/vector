#include <pw/impl/is_constructible.h>
//#include <type_traits>

#include <catch2/catch.hpp>

SCENARIO("is_constructible", "[type_traits]")
{
    GIVEN("A default constructible object")
    {
        struct Example
        {
            int a = 0;
        };
        WHEN("Call is_constructibe() with invalid args")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible<Example, int>::value);
            }
        }
        WHEN("Call is_constructibe() with no args")
        {
            THEN("it is")
            {
                REQUIRE(pw::is_constructible<Example>::value);
            }
        }
    }
    GIVEN("An object with constructor")
    {
        struct Example2
        {
            Example2(float v)
                : a(3)
            {
            }
            int a = 0;
        };
        WHEN("Call is_constructibe() with default constructor")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible<Example2>::value);
            }
        }
        WHEN("Call is_constructibe() with invalid args")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible<Example2, char*>::value);
            }
        }
        WHEN("Call is_constructibe() with correct args")
        {
            THEN("it is")
            {
                REQUIRE(pw::is_constructible<Example2, float>::value);
            }
        }
    }
}
