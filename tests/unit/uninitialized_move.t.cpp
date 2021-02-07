#include <catch2/catch.hpp>
#include <pw/impl/uninitialized_move.h>

#include <test_copyconstructible.h>
#include <test_opcounter.h>
#include <test_permute.h>

SCENARIO("check unitialized_move()", "[uninitialized_move]")
{
    size_t const                count        = 3;
    pw::test::CopyConstructible array[count] = {
        pw::test::CopyConstructible(2),
        pw::test::CopyConstructible(1),
        pw::test::CopyConstructible(3),
    };
    pw::test::CopyConstructible dest[count] = { 10, 11, 12 };
    pw::test::OpCounter         init        = pw::test::CopyConstructible::getCounter();
    GIVEN("A src and dst array")
    {
        WHEN("move")
        {
            pw::uninitialized_move(&array[0], &array[count], &dest[0]);
            THEN("move constructor called; destructor not")
            {
                pw::test::OpCounter count = pw::test::CopyConstructible::getCounter() - init;
                REQUIRE(3 == count.getMoveConstructor());
                REQUIRE(count.constructorCount() == count.getMoveConstructor());
            }
        }
    }
}
