#include "catch2/catch.hpp"
#include "optracker.h"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("Test erase", "[vector][erase]", TestTypeList)
{
    using Vector = TestType;
    typename Vector::iterator iter;
    using value_type = typename Vector::value_type;
    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("erase() at begin()")
        {
            iter = v.erase(v.begin());
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
        WHEN("erase() at end()")
        {
            iter = v.erase(v.end());
            THEN("nothing changed")
            {
                REQUIRE(v.empty());
            }
            THEN("ends are the same")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(v.begin() == v.end());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        Values<Vector> generate(5);
        Vector&        v = generate.values;

        WHEN("erase() at begin()")
        {
            typename Vector::iterator iter;

            iter = v.erase(v.begin());
            THEN("one less value")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(generate.count - 1 == v.size());
                REQUIRE(generate.last_value == v[generate.count - 2]);
            }
        }
        WHEN("erase(begin,end)")
        {
            typename Vector::iterator iter;

            iter = v.erase(v.begin(), v.end());
            THEN("empty")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(v.empty());
            }
        }
        WHEN("erase() at begin() to begin()+3")
        {
            typename Vector::iterator iter;
            iter = v.erase(v.begin(), v.end() - 2);
            THEN("there are 2 values")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(2 == v.size());
                REQUIRE(generate.last_value == v[1]);
            }
        }
    }
}
