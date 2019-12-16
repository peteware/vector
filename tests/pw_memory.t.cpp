#include <pw/memory>

#include <catch2/catch.hpp>

SCENARIO("validate pointer_traits works as expected")
{
    GIVEN("A pointer_traits with an int*")
    {
        pw::pointer_traits<int*>::pointer a;
        int*                              b;
        WHEN("n int is used")
        {
            THEN("it's the same type") { REQUIRE(a == b); }
        }
    }
    GIVEN("A pointer_traits with a struct pointer")
    {
        struct FakeAllocator
        {
            using pointer = char;
        };

        pw::pointer_traits<FakeAllocator>::pointer a;
        char                                       b;
        WHEN("a pointerused")
        {
            THEN("it's the same type") { REQUIRE(a == b); }
        }
    }
}
