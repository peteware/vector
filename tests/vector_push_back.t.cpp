#include "catch2/catch.hpp"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("push_back()", "[vector][pushback]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
    }
    GIVEN("A vector with 5 elements")
    {
        size_t const count = 5;
        value_type   first_value;
        value_type   last_value;
        value_type   value;
        Vector       values;

        for (size_t i = 0; i < count; ++i)
        {
            permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
            last_value = value;
        }
        Vector v(values);

        WHEN("push_back() const_ref is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                value_type t;
                v.push_back(t);
            }
            REQUIRE(capacity < v.capacity());
        }
        WHEN("push_back() move is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                v.push_back(value_type());
            }
            REQUIRE(capacity < v.capacity());
        }
    }
}
