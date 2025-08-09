#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

#include <pw/algorithm>

/*
 * Type requirements:
 * - No extra
 */
TEMPLATE_LIST_TEST_CASE("Test assign()", "[vector][assign]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("assign(count,value) and lhs is empty")
        {
            value_type value;
            size_t     count = 12;

            pw::test::permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::test::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(count,value) and lhs items")
        {
            value_type value;
            size_t     count = 12;
            v.resize(count - 3);

            pw::test::permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::test::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(begin,end)")
        {
            value_type   value;
            size_t const count         = 3;
            value_type   values[count] = { pw::test::permute_n(value, 4, 1),
                                           pw::test::permute_n(value, 4, 1),
                                           pw::test::permute_n(value, 4, 1) };

            v.assign(&values[0], &values[count]);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are same")
            {
                REQUIRE(pw::equal(&values[0], &values[count], v.begin(), v.end()));
            }
        }
        WHEN("assign(init_list)")
        {
            value_type value;
            v.assign({ pw::test::permute_n(value, 4, 1),
                       pw::test::permute_n(value, 4, 1),
                       pw::test::permute_n(value, 4, 1) });
            THEN("size() is 3")
            {
                REQUIRE(static_cast<size_t>(3) == v.size());
            }
        }
    }
}
