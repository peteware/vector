#include <pw/impl/utility/move.h>

#include <test_opcounter.h>
#include <test_optracker_copyconstructible.h>
#include <test_optracker_defaultcopyconstructible.h>
#include <test_optracker_moveconstructible.h>
#include <test_testtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_LIST_TEST_CASE("move", "[move]", pw::test::Phase2TestTypeList)
{
    GIVEN("A OpTrackerMoveConstructible object")
    {
        pw::test::OpTrackerMoveConstructible m;
        pw::test::OpCounter                  init = pw::test::OpTrackerMoveConstructible::getCounter();

        WHEN("An lvalue is move()")
        {
            pw::test::OpTrackerMoveConstructible m2      = pw::move(m);
            pw::test::OpCounter                  counter = m2.getCounter() - init;
            THEN("move construct is called")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
    }
    GIVEN("A OpTrackerCopyConstructible object")
    {
        pw::test::OpTrackerCopyConstructible m(9);
        pw::test::OpCounter                  init = m.getCounter();

        WHEN("It is copied")
        {
            pw::test::OpTrackerCopyConstructible m2(m); // NOLINT(*-unnecessary-copy-initialization)
            pw::test::OpCounter                  counter = m2.getCounter() - init;
            THEN("copy constructor is called")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getCopyConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
        WHEN("An lvalue is move()")
        {
            pw::test::OpTrackerCopyConstructible m2(pw::move(m));
            pw::test::OpCounter                  counter = m2.getCounter() - init;
            THEN("move construct is called")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
    }
}
