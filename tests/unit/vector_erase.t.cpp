#include <test_testtype.h>
#include <test_values.h>

#include <catch2/catch.hpp>

/*
 * Type Requirments
 * - erase(): MoveAssignable
 */
TEMPLATE_LIST_TEST_CASE("Test erase", "[vector][erase]", pw::test::TestTypeList)
{
    using Vector = TestType;
    typename Vector::iterator iter;
    using value_type = typename Vector::value_type;
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
        pw::test::Values<Vector> generate(5);
        Vector&                  v = generate.values;

        WHEN("erase() at begin()")
        {
            typename Vector::iterator iter;

            iter = v.erase(v.begin());
            THEN("one less value")
            {
                REQUIRE(generate.count - 1 == v.size());
                REQUIRE(v[0] != generate.first_value);
                REQUIRE(generate.last_value == v[generate.count - 2]);
                REQUIRE(v.begin() == iter);
            }
        }
        WHEN("erase() in middle")
        {
            typename Vector::iterator iter;

            iter = v.erase(v.begin() + 1);
            THEN("one less value")
            {
                REQUIRE(generate.count - 1 == v.size());
                REQUIRE(v[0] == generate.first_value);
                REQUIRE(generate.last_value == v[generate.count - 2]);
                REQUIRE(v.begin() + 1 == iter);
            }
        }
        WHEN("erase() last item")
        {
            typename Vector::iterator iter;

            iter = v.erase(v.end() - 1);
            THEN("one less value")
            {
                REQUIRE(v[0] == generate.first_value);
                REQUIRE(generate.count - 1 == v.size());
                REQUIRE(v.end() == iter);
            }
        }
        WHEN("erase(begin,end)")
        {
            typename Vector::iterator iter;

            iter = v.erase(v.begin(), v.end());
            THEN("empty")
            {
                REQUIRE(v.end() == iter);
                REQUIRE(v.empty());
            }
        }
        WHEN("erase() at begin() to begin()+3")
        {
            typename Vector::iterator iter;
            iter = v.erase(v.begin(), v.end() - 2);
            THEN("there are 2 values")
            {
                REQUIRE(2 == v.size());
                REQUIRE(generate.last_value == v[1]);
                REQUIRE(v.begin() == iter);
            }
        }
        WHEN("erase() at begin()+2 to end()")
        {
            typename Vector::iterator iter;
            iter = v.erase(v.begin() + 2, v.end());
            THEN("there are 3 fewer values")
            {
                REQUIRE(generate.count - 3 == v.size());
                REQUIRE(generate.first_value == v[0]);
                REQUIRE(v.begin() + 2 == iter);
            }
        }
    }
}
