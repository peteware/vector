#include <pw/impl/allocator_traits.h>

#include <pw/impl/allocator.h>

#include "optracker.h"

#include "catch2/catch.hpp"

SCENARIO("Allocator traits", "[allocator_traits]")
{
    GIVEN("An allocator of int")
    {
        pw::allocator<int> alloc;
        int                x;

        WHEN("construct that int")
        {
            pw::allocator_traits<pw::allocator<int>>::construct(alloc, &x, 3);
            THEN("value is set")
            {
                REQUIRE(3 == x);
            }
        }
    }
    GIVEN("An allocator of EmplaceMoveConstructible")
    {
        pw::allocator<EmplaceMoveConstructible> alloc;
        EmplaceMoveConstructible                x(4, 5);

        WHEN("construct ")
        {
            pw::allocator_traits<pw::allocator<EmplaceMoveConstructible>>::construct(alloc, &x, 6, 8);
            THEN("value is set")
            {
                REQUIRE(6 == x.value());
                REQUIRE(8 == x.value2());
            }
        }
    }
}
