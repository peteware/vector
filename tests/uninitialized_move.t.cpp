#include "catch2/catch.hpp"
#include "conscounter.h"
#include "copyconstructible.h"
#include "permute.h"
#include <pw/impl/uninitialized_move.h>

SCENARIO("check unitialized_move()", "[uninitialized_move]")
{
    size_t const      count        = 3;
    CopyConstructible array[count] = {
        CopyConstructible(2),
        CopyConstructible(1),
        CopyConstructible(3),
    };
    CopyConstructible dest[count];
    ConsCounter       init = CopyConstructible::getCounter();
    GIVEN("A src and dst array")
    {
        WHEN("move")
        {
            pw::uninitialized_move(&array[0], &array[count], &dest[0]);
            THEN("move constructor called; destructor not")
            {
                ConsCounter count = CopyConstructible::getCounter() - init;
                REQUIRE(3 == count.getMoveConstructor());
                REQUIRE(count.constructorCount() == count.getMoveConstructor());
            }
        }
    }
}
