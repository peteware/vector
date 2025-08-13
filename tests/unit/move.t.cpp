#include <pw/impl/move.h>

#include <test_copyconstructible.h>
#include <test_defaultcopyconstructible.h>
#include <test_moveconstructible.h>
#include <test_opcounter.h>
#include <test_testtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_LIST_TEST_CASE("move", "[move]", pw::test::Phase2TestTypeList)
{
    GIVEN("A MoveConstructible object")
    {
        pw::test::MoveConstructible m;
        pw::test::OpCounter         init = pw::test::MoveConstructible::getCounter();

        WHEN("An lvalue is move()")
        {
            pw::test::MoveConstructible m2      = pw::move(m);
            pw::test::OpCounter         counter = m2.getCounter() - init;
            THEN("move construct is called")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
    }
    GIVEN("A CopyConstructible object")
    {
        pw::test::CopyConstructible m(9);
        pw::test::OpCounter         init = m.getCounter();

        WHEN("It is copied")
        {
            pw::test::CopyConstructible m2(m);
            pw::test::OpCounter         counter = m2.getCounter() - init;
            THEN("copy constructor is called")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getCopyConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
        WHEN("An lvalue is move()")
        {
            pw::test::CopyConstructible m2(pw::move(m));
            pw::test::OpCounter         counter = m2.getCounter() - init;
            THEN("move construct is called")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
    }
}
