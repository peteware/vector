#include "catch2/catch.hpp"
#include "optracker.h"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

using TestIntList = std::tuple<pw::vector<int>, std::vector<int>>;

TEMPLATE_LIST_TEST_CASE("emplace_back()", "[vector][emplace_back]", TestIntList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        v.emplace_back(3);
    }
    GIVEN("A vector with 5 elements")
    {
        Values<Vector> generate(5);
        Vector&        v = generate.values;

        WHEN("emplace_back()")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                v.emplace_back(i);
            }
            REQUIRE(capacity < v.capacity());
        }
    }
}

SCENARIO("emplace_back() op counts", "[vector][emplace_back][optracker]")
{
    using Vector     = pw::vector<CopyConstructible>;
    using value_type = typename Vector::value_type;
    OpCounter       counter;
    OpCounter const init = CopyConstructible::getCounter();

    GIVEN("An empty vector")
    {
        Vector            v;
        CopyConstructible copyObject;

        WHEN("emplace_back() is called")
        {
            counter = CopyConstructible::getCounter();
            v.emplace_back(copyObject);
            THEN("Copy construct called once")
            {
                counter = CopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.constructorCount());
                REQUIRE(counter.constructorCount() == counter.allCount());
            }
        }
        WHEN("emplace_back(move) is called")
        {
            counter = CopyConstructible::getCounter();
            v.emplace_back(pw::move(copyObject));
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
        WHEN("emplace_back() is called")
        {
            generate.values.emplace_back(copyObject);
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
