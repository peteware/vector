#include "catch2/catch.hpp"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("Test assign()", "[vector][assign]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("assign(count,value) and lhs is empty")
        {
            value_type value;
            size_t     count = 12;

            permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::internal::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(count,value) and lhs items")
        {
            value_type value;
            size_t     count = 12;
            v.resize(count - 3);

            permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::internal::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(begin,end)")
        {
            value_type   value;
            size_t const count         = 3;
            value_type   values[count] = { permute_n(value, 4, 1),
                                         permute_n(value, 4, 1),
                                         permute_n(value, 4, 1) };

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
            v.assign({ permute_n(value, 4, 1), permute_n(value, 4, 1), permute_n(value, 4, 1) });
            THEN("size() is 3")
            {
                REQUIRE(static_cast<size_t>(3) == v.size());
            }
        }
    }
}