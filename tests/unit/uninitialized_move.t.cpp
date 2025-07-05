#include <catch2/catch.hpp>

#include <pw/impl/equal.h>
#include <pw/impl/uninitialized_move.h>

#include <test_copyconstructible.h>
#include <test_opcounter.h>
#include <test_permute.h>

SCENARIO("check unitialized_move()", "[uninitialized_move]")
{
    GIVEN("A src and dst array")
    {
        size_t const                count        = 3;
        pw::test::CopyConstructible array[count] = {
            pw::test::CopyConstructible(2),
            pw::test::CopyConstructible(1),
            pw::test::CopyConstructible(3),
        };
        pw::test::CopyConstructible dest[count] = { 10, 11, 12 };
        pw::test::OpCounter init = pw::test::CopyConstructible::getCounter();
        WHEN("move")
        {
            pw::uninitialized_move(&array[0], &array[count], &dest[0]);
            THEN("move constructor called; destructor not")
            {
                pw::test::OpCounter count =
                    pw::test::CopyConstructible::getCounter() - init;
                REQUIRE(3 == count.getMoveConstructor());
                REQUIRE(0 == count.getCopyConstructor());
                REQUIRE(count.constructorCount() == count.getMoveConstructor());
                REQUIRE(0 == count.destructorCount());
            }
            THEN("values are moved, too")
            {
                REQUIRE(2 == dest[0].value());
                REQUIRE(1 == dest[1].value());
                REQUIRE(3 == dest[2].value());
            }
        }
    }
    GIVEN("Arrays of int")
    {
        int const src[] = { 1, 2, 3 };
        int       dst[] = { 4, 5, 6 };

        WHEN("move the int from src to dst")
        {
            pw::uninitialized_move(&src[0], &src[3], &dst[0]);
            THEN("src is unchanged")
            {
                REQUIRE(src[0] == 1);
            }

            THEN("src and dst same")
            {
                REQUIRE(pw::equal(&dst[0], &dst[3], &src[0], &src[3]));
            }
        }
    }
}
