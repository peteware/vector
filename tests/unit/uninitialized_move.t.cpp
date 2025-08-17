#include <pw/impl/equal.h>
#include <pw/impl/uninitialized_move.h>
#include <test_copyconstructible.h>
#include <test_opcounter.h>
#include <test_throwingtype.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <pw/internal/rsize_fix.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("check unitialized_move()", "[uninitialized_move]")
{
    GIVEN("A src and dst array")
    {
        constexpr size_t            count        = 3;
        pw::test::CopyConstructible array[count] = {
            pw::test::CopyConstructible(2),
            pw::test::CopyConstructible(1),
            pw::test::CopyConstructible(3),
        };
        pw::test::CopyConstructible dest[count] = { 10, 11, 12 };
        pw::test::OpCounter         init        = pw::test::CopyConstructible::getCounter();
        WHEN("move")
        {
            pw::uninitialized_move(&array[0], &array[count], &dest[0]);
            THEN("move constructor called; destructor not")
            {
                pw::test::OpCounter count = pw::test::CopyConstructible::getCounter() - init;
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
        constexpr int src[] = { 1, 2, 3 };
        int           dst[] = { 4, 5, 6 };
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
    GIVEN("Two arrays of ThrowingType")
    {
        using value_type = pw::test::ThrowingType;
        value_type::reset();
        value_type src[5] = { value_type(1), value_type(2), value_type(3), value_type(4), value_type(5) };
        value_type dest[5];
        value_type::reset();
        WHEN("move the ThrowingType from src to dest")
        {
            value_type::throw_after_n = 2;
            REQUIRE_THROWS(pw::uninitialized_move(&src[0], &src[5], &dest[0]));
            THEN("move constructor called; destructor not")
            {
                REQUIRE(value_type::construction_count == 0);
            }
            THEN("values are moved, too")
            {
                REQUIRE(src[0].value == -1);  // ThrowingType move constructor sets value to -other.value
                REQUIRE(src[1].value == -2);  // ThrowingType move constructor sets value to -other.value
                REQUIRE(src[2].value == -3);  // ThrowingType move constructor sets value to -other.value
                REQUIRE(dest[0].value == -2); // ThrowingType destructor sets value to -2
                REQUIRE(dest[1].value == -2); // ThrowingType destructor sets value to -2
                REQUIRE(dest[2].value == 3);  // Sets value = other.value before exception is raised
                REQUIRE(dest[3].value == 0);
                REQUIRE(dest[4].value == 0);
            }
        }
    }
}
