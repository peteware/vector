#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>

#include <test_emplacemoveconstructible.h>

#include <catch2/catch_test_macros.hpp>

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
        pw::allocator<pw::test::EmplaceMoveConstructible> alloc;
        pw::test::EmplaceMoveConstructible                x(4, 5);

        WHEN("construct")
        {
            pw::allocator_traits<pw::allocator<pw::test::EmplaceMoveConstructible>>::construct(
                alloc, &x, 6, 8);
            THEN("value is set")
            {
                REQUIRE(6 == x.value());
                REQUIRE(8 == x.value2());
            }
        }
    }
}
