#include <pw/impl/is_empty.h>

#include <catch2/catch.hpp>

SCENARIO("is_empty", "[type_traits]")
{
    struct Empty
    {
    };
    GIVEN("An empty struct")
    {
        THEN("is_empty is true") { REQUIRE(pw::is_empty<Empty>::value); }
        THEN("is_empty_v is true") { REQUIRE(pw::is_empty_v<Empty>); }
    }
}
