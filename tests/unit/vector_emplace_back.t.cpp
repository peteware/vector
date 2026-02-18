#include <test_testtype.h>
#include <test_values.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_LIST_TEST_CASE("emplace_back()", "[vector][func][emplace_back]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector v;

        WHEN("emplace_back() one element")
        {
            value_type val {};
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
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> gen(5);
        Vector                   v { gen.values[0], gen.values[1], gen.values[2] };

        WHEN("emplace_back() without enough capacity")
        {
            REQUIRE(v.capacity() == v.size());
            v.emplace_back(gen.last_value);
            THEN("capacity increased")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("last element is as expected")
            {
                REQUIRE(v.back() == gen.last_value);
            }
        }
    }
    WHEN("emplace_back() with enough capacity")
    {
        pw::test::Values<Vector> gen(5);
        Vector                   v { gen.values[0], gen.values[1], gen.values[2] };

        v.reserve(v.size() + 1);
        REQUIRE(v.capacity() > v.size());

        v.emplace_back(gen.last_value);
        THEN("last element is as expected")
        {
            REQUIRE(v.back() == gen.last_value);
        }
    }
}

// using TestIntList = std::tuple<pw::vector<int>, std::vector<int>>;
// // using TestEmplaceList = std::tuple<pw::vector<pw::test::OpTrackerEmplaceMoveConstructible>,
// //                                    std::vector<pw::test::OpTrackerEmplaceMoveConstructible>>;
// using TestEmplaceList = std::tuple<pw::vector<pw::test::OpTrackerEmplaceMoveConstructible>>;

// /*
//  * Type requirements:
//  * - emplace_back(): MoveInsertable and EmplaceConstructible
//  * - emplace(): MoveAssignable, MoveInsertable and EmplaceConstructible
//  */
// TEMPLATE_LIST_TEST_CASE("emplace_back() with OpTrackerEmplaceMoveConstructible",
//                         "[vector][emplace_back]",
//                         TestEmplaceList)
// {
//     using Vector     = TestType;
//     using value_type = typename Vector::value_type;

//     GIVEN("An empty vector")
//     {
//         Vector              v;
//         pw::test::OpCounter init = pw::test::OpTrackerEmplaceMoveConstructible::getCounter();
//         pw::test::OpCounter counter;

//         WHEN("emplace-back() an element")
//         {
//             v.emplace_back(3, 4);
//             counter = pw::test::OpTrackerEmplaceMoveConstructible::getCounter() - init;
//             THEN("size() is 1") { REQUIRE(1 == v.size()); }
//             THEN("element has correct values")
//             {
//                 REQUIRE(3 == v.front().value());
//                 REQUIRE(4 == v.front().value2());
//             }
//             THEN("other constructed, not copy or moce")
//             {
//                 REQUIRE(1 == counter.getOtherConstructor());
//                 REQUIRE(1 == counter.constructorCount());
//             }
//         }
//     }
//     GIVEN("A vector with elements")
//     {
//         Vector              v    = { { 1, 2 }, { 3, 4 }, { 4, 5 } };
//         pw::test::OpCounter init = pw::test::OpTrackerEmplaceMoveConstructible::getCounter();
//         pw::test::OpCounter counter;

//         REQUIRE(3 == v.size());
//         WHEN("emplace-back() an element")
//         {
//             v.emplace_back(13, 14);
//             counter = pw::test::OpTrackerEmplaceMoveConstructible::getCounter() - init;
//             THEN("size() is 4") { REQUIRE(4 == v.size()); }
//             THEN("element has correct values")
//             {
//                 REQUIRE(1 == v.front().value());
//                 REQUIRE(2 == v.front().value2());
//                 REQUIRE(13 == v.back().value());
//                 REQUIRE(14 == v.back().value2());
//             }
//             THEN("other constructed, not copy or move")
//             {
//                 INFO("counter: " << counter);
//                 REQUIRE(1 == counter.getOtherConstructor());
//                 REQUIRE(3 == counter.getMoveConstructor());
//                 REQUIRE(4 == counter.constructorCount());
//             }
//         }
//     }
// }
