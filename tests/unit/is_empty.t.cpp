#include <pw/impl/is_empty.h>

#include <catch2/catch_all.hpp>

SCENARIO("is_empty", "[type_traits]")
{
    GIVEN("An empty struct")
    {
        struct Empty
        {
        };
        THEN("is_empty is true")
        {
            REQUIRE(pw::is_empty<Empty>::value);
        }
        THEN("is_empty_v is true")
        {
            REQUIRE(pw::is_empty_v<Empty>);
        }
    }
    GIVEN("An non-empty struct")
    {
        struct NonEmpty
        {
            int a;
        };
        THEN("is_empty is false")
        {
            REQUIRE(!pw::is_empty<NonEmpty>::value);
        }
        THEN("is_empty_v is false")
        {
            REQUIRE(!pw::is_empty_v<NonEmpty>);
        }
    }
    GIVEN("A struct with virtual destructor")
    {
        struct Virt
        {
            virtual ~Virt();
        };
        THEN("is_empty is false")
        {
            REQUIRE(!pw::is_empty<Virt>::value);
        }
        THEN("is_empty_v is false")
        {
            REQUIRE(!pw::is_empty_v<Virt>);
        }
    }
    GIVEN("A union")
    {
        union Union
        {
        };
        THEN("is_empty is false")
        {
            REQUIRE(!pw::is_empty<Union>::value);
        }
        THEN("is_empty_v is false")
        {
            REQUIRE(!pw::is_empty_v<Union>);
        }
    }
    GIVEN("An in")
    {
        THEN("is_empty is false")
        {
            REQUIRE(!pw::is_empty<int>::value);
        }
        THEN("is_empty_v is false")
        {
            REQUIRE(!pw::is_empty_v<int>);
        }
    }
}
