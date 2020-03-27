#include <pw/memory>
#include <pw/type_traits>

#include "catch2/catch.hpp"

struct FakeAllocator
{
    using element_type    = int;
    using difference_type = char;
};

SCENARIO("validate pointer_traits works as expected", "[memory]")
{
    GIVEN("A pointer_traits with an int*")
    {
        WHEN("an int* is used")
        {
            THEN("it's the same type")
            {
                REQUIRE(pw::is_same<pw::pointer_traits<int*>::pointer, int*>::value);
            }
        }
    }
    GIVEN("A pointer_traits with a struct pointer")
    {
        WHEN("a pointer used")
        {
            THEN("it's the same type")
            {
                REQUIRE(pw::is_same<pw::pointer_traits<FakeAllocator>::pointer, FakeAllocator>::value);
            }
        }
    }
}
