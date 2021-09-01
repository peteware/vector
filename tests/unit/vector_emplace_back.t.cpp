#include <test_emplacemoveconstructible.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

TEMPLATE_LIST_TEST_CASE("emplace_back()", "[vector][func][emplace_back]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector     v;
        value_type val;
        WHEN("emplace_back() one element")
        {
            v.emplace_back(val);
            THEN("size() == 1")
            {
                REQUIRE(v.size() == 1);
            }
            THEN("front() == val")
            {
                REQUIRE(v.front() == val);
            }
        }
    }
}

using TestIntList = std::tuple<pw::vector<int>, std::vector<int>>;
// using TestEmplaceList = std::tuple<pw::vector<pw::test::EmplaceMoveConstructible>,
//                                    std::vector<pw::test::EmplaceMoveConstructible>>;
using TestEmplaceList = std::tuple<pw::vector<pw::test::EmplaceMoveConstructible>>;

/*
 * Type requirements:
 * - emplace_back(): MoveInsertable and EmplaceConstructible
 * - emplace(): MoveAssignable, MoveInsertable and EmplaceConstructible
 */
TEMPLATE_LIST_TEST_CASE("emplace_back() with EmplaceMoveConstructible",
                        "[vector][emplace_back]",
                        TestEmplaceList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector              v;
        pw::test::OpCounter init = pw::test::EmplaceMoveConstructible::getCounter();
        pw::test::OpCounter counter;

        WHEN("emplace-back() an element")
        {
            v.emplace_back(3, 4);
            counter = pw::test::EmplaceMoveConstructible::getCounter() - init;
            THEN("size() is 1") { REQUIRE(1 == v.size()); }
            THEN("element has correct values")
            {
                REQUIRE(3 == v.front().value());
                REQUIRE(4 == v.front().value2());
            }
            THEN("other constructed, not copy or moce")
            {
                REQUIRE(1 == counter.getOtherConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
    }
    GIVEN("A vector with elements")
    {
        Vector              v    = { { 1, 2 }, { 3, 4 }, { 4, 5 } };
        pw::test::OpCounter init = pw::test::EmplaceMoveConstructible::getCounter();
        pw::test::OpCounter counter;

        REQUIRE(3 == v.size());
        WHEN("emplace-back() an element")
        {
            v.emplace_back(13, 14);
            counter = pw::test::EmplaceMoveConstructible::getCounter() - init;
            THEN("size() is 4") { REQUIRE(4 == v.size()); }
            THEN("element has correct values")
            {
                REQUIRE(1 == v.front().value());
                REQUIRE(2 == v.front().value2());
                REQUIRE(13 == v.back().value());
                REQUIRE(14 == v.back().value2());
            }
            THEN("other constructed, not copy or move")
            {
                INFO("counter: " << counter);
                REQUIRE(1 == counter.getOtherConstructor());
                REQUIRE(3 == counter.getMoveConstructor());
                REQUIRE(4 == counter.constructorCount());
            }
        }
    }
}
