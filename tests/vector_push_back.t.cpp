#include "catch2/catch.hpp"
#include "optracker.h"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("push_back()", "[vector][push_back]", TestTypeList)
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

SCENARIO("push_back() op counts", "[vector][push_back][optracker]")
{
    using Vector     = pw::vector<CopyConstructible>;
    using value_type = typename Vector::value_type;
    OpCounter       counter;
    OpCounter const init = CopyConstructible::getCounter();

    GIVEN("An empty vector")
    {
        Vector            v;
        CopyConstructible copyObject;

        WHEN("push_back() is called")
        {
            counter = CopyConstructible::getCounter();
            v.push_back(copyObject);
            THEN("Copy construct called once")
            {
                counter = CopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.constructorCount());
                REQUIRE(counter.constructorCount() == counter.allCount());
            }
        }
        WHEN("push_back(move) is called")
        {
            counter = CopyConstructible::getCounter();
            v.push_back(pw::move(copyObject));
            THEN("Copy construct called once")
            {
                counter = CopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(counter.getMoveConstructor() == counter.allCount());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        Values<Vector>    generate(5);
        CopyConstructible copyObject;
        OpCounter         startCount(CopyConstructible::getCounter());

        generate.values.shrink_to_fit();
        counter = CopyConstructible::getCounter();
        WHEN("push_back() is called")
        {
            generate.values.push_back(copyObject);
            counter = CopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items")
            {
                REQUIRE(generate.count == counter.getMoveConstructor());
            }
            THEN("Copy constructed new item")
            {
                REQUIRE(1 == counter.getCopyConstructor());
            }
        }
    }
    counter = CopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}
