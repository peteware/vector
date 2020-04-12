#include "catch2/catch.hpp"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("Test insert", "[vector][insert]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("insert() is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            permute(value, 3);
            iter = v.insert(v.begin(), value);
            THEN("size() is increased")
            {
                REQUIRE(1 == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
            }
            THEN("at(0) returns same value")
            {
                REQUIRE(value == v.at(0));
            }
        }
        WHEN("insert(count) at begin()")
        {
            typename Vector::iterator iter;
            value_type                value;
            size_t                    count = 12;

            permute(value, 3);
            iter = v.insert(v.begin(), count, value);
            THEN("there are count values")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(count == v.size());
                REQUIRE(value == v[0]);
                REQUIRE(value == v[count - 1]);
            }
        }
        WHEN("insert(12) at end()")
        {
            typename Vector::iterator iter;
            value_type                value;
            size_t                    count = 12;

            permute(value, 3);
            iter = v.insert(v.end(), count, value);
            THEN("there are count values")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(count == v.size());
                REQUIRE(value == v[0]);
                REQUIRE(value == v[count - 1]);
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        size_t const              count = 5;
        value_type                first_value;
        value_type                last_value;
        value_type                value;
        size_t                    added;
        size_t                    offset;
        typename Vector::iterator where;
        Vector                    values;

        for (size_t i = 0; i < count; ++i)
        {
            permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
            last_value = value;
        }
        Vector v(values);
        WHEN("insert(3) at begin")
        {
            added  = 3;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(3) at end")
        {
            added  = 3;
            offset = v.size();
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at begin")
        {
            added  = 10;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at near end")
        {
            added  = 10;
            offset = count - 1;
            where  = v.begin() + offset;
            where  = v.insert(where, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
    }
}
