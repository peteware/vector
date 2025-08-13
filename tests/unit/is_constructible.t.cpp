#include <pw/impl/is_constructible.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("is_constructible", "[type_traits]")
{
    GIVEN("A default constructible object")
    {
        struct Example
        {
            int a = 0;
        };
        WHEN("Call is_constructible() with invalid args")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible<Example, char*>::value);
            }
        }
        WHEN("Call is_constructibe_v with invalid args")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible_v<Example, char*>);
            }
        }
        WHEN("Call is_constructibe() with no args")
        {
            THEN("it is")
            {
                REQUIRE(pw::is_constructible<Example>::value);
            }
        }
        WHEN("Call is_constructibe_v with no args")
        {
            THEN("it is")
            {
                REQUIRE(pw::is_constructible_v<Example>);
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
        WHEN("Call is_constructibe_v with default constructor")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible_v<Example2>);
            }
        }
        WHEN("Call is_constructibe() with invalid args")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible<Example2, char*>::value);
            }
        }
        WHEN("Call is_constructibe_v with invalid args")
        {
            THEN("it is not")
            {
                REQUIRE(!pw::is_constructible_v<Example2, char*>);
            }
        }
        WHEN("Call is_constructibe() with correct args")
        {
            THEN("it is")
            {
                REQUIRE(pw::is_constructible<Example2, float>::value);
            }
        }
        WHEN("Call is_constructibe_v with correct args")
        {
            THEN("it is")
            {
                REQUIRE(pw::is_constructible_v<Example2, float>);
            }
        }
    }
}
