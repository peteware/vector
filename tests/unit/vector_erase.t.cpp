#include <test_testtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

/*
 * Type Requirments
 * - erase(): MoveAssignable
 */
TEMPLATE_LIST_TEST_CASE("Test erase", "[vector][erase]", pw::test::TestTypeListInt)
{
    using Vector = TestType;
    typename Vector::iterator iter;

    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("erase(begin,end)")
        {
            iter = v.erase(v.begin(), v.end());
            THEN("nothing is changed")
            {
                REQUIRE(v.empty());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(v.begin() == v.end());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        Vector v { 1, 2, 3, 4, 5 };

        WHEN("erase() at begin()")
        {
            Vector expected { 2, 3, 4, 5 };
            auto   iter = v.erase(v.begin());
            THEN("one less value")
            {
                REQUIRE(expected == v);
                //     REQUIRE(generate.count - 1 == v.size());
                //     REQUIRE(v.begin() == iter);
                //     REQUIRE(v[0] != generate.first_value);
                //     REQUIRE(generate.last_value == v[generate.count - 2]);
            }
        }
        WHEN("erase() in middle")
        {
            Vector expected { 1, 3, 4, 5 };

            iter = v.erase(v.begin() + 1);
            THEN("one less value")
            {
                REQUIRE(expected == v);
                // REQUIRE(generate.count - 1 == v.size());
                // REQUIRE(v[0] == generate.first_value);
                // REQUIRE(generate.last_value == v[generate.count - 2]);
                // REQUIRE(v.begin() + 1 == iter);
            }
        }
        WHEN("erase() last item")
        {
            Vector expected { 1, 2, 3, 4 };
            auto   iter = v.erase(v.end() - 1);
            THEN("one less value")
            {
                REQUIRE(expected == v);
                // REQUIRE(v[0] == generate.first_value);
                // REQUIRE(generate.count - 1 == v.size());
                // REQUIRE(v.end() == iter);
            }
        }
        WHEN("erase(begin,end)")
        {
            auto iter = v.erase(v.begin(), v.end());
            THEN("empty")
            {
                REQUIRE(v.end() == iter);
                REQUIRE(v.empty());
            }
        }
        WHEN("erase() at begin() to begin()+3")
        {
            Vector expected { 4, 5 };
            auto   iter = v.erase(v.begin(), v.end() - 2);
            THEN("there are 2 values")
            {
                REQUIRE(expected == v);
                REQUIRE(v.begin() == iter);
            }
        }
        WHEN("erase() at begin()+2 to end()")
        {
            Vector expected { 1, 2 };
            auto   iter = v.erase(v.begin() + 2, v.end());
            THEN("there are 3 fewer values")
            {
                REQUIRE(expected == v);
                REQUIRE(v.begin() + 2 == iter);
            }
        }
    }
}
