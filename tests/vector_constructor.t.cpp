#include <pw/vector>

#include "test_optracker.h"

#include <stdexcept>
#include <tuple>
#include <vector>

#include "catch2/catch.hpp"

using TestTypeList =
    std::tuple<pw::vector<pw::test::CopyConstructible>, std::vector<pw::test::CopyConstructible>>;
//using TestTypeList = std::tuple<std::vector<pw::test::CopyConstructible>>;
TEMPLATE_LIST_TEST_CASE("count constructors in vector", "[vector][constructor]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init(pw::test::CopyConstructible::getCounter());

    GIVEN("An empty vector")
    {
        Vector       v;
        size_t const count = 5;

        WHEN("Nothing was called")
        {
            counter = pw::test::CopyConstructible::getCounter() - init;
            THEN("Nothing was constructed")
            {
                REQUIRE(counter.zero());
            }
        }
        WHEN("reserve() is increased")
        {
            counter = pw::test::CopyConstructible::getCounter();
            v.reserve(5);
            counter = pw::test::CopyConstructible::getCounter() - counter;
            THEN("copy constructor not called")
            {
                REQUIRE(counter.zero());
            }
        }
    }
    GIVEN("and add count objects")
    {
        size_t const                count = 5;
        pw::test::CopyConstructible copyObject;
        pw::test::OpCounter         startCount(pw::test::CopyConstructible::getCounter());
        Vector                      v(count);

        counter = pw::test::CopyConstructible::getCounter();

        REQUIRE(v.size() == v.capacity());
        WHEN("getCounter()")
        {
            counter = pw::test::CopyConstructible::getCounter() - startCount;
            THEN("count items default constructed")
            {
                REQUIRE(count == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() == counter.constructorCount());
            }
        }
    }
    counter = pw::test::CopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

TEMPLATE_LIST_TEST_CASE("init-list", "[vector][constructor][init-list]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter               init(pw::test::CopyConstructible::getCounter());
    pw::test::OpCounter               counter;
    std::initializer_list<value_type> initlist = { 1, 2, 5 };

    counter = pw::test::CopyConstructible::getCounter() - init;
    REQUIRE(3 == counter.constructorCount());
    REQUIRE(3 == counter.getDefaultConstructor());
    GIVEN("A vector from an init list")
    {
        init = pw::test::CopyConstructible::getCounter();
        Vector v { initlist };
        WHEN("nothing is changed")
        {
            THEN("size() is same")
            {
                REQUIRE(3 == v.size());
            }
            THEN("capacity() is same")
            {
                REQUIRE(v.size() == v.capacity());
            }
            THEN("constructors called")
            {
                counter = pw::test::CopyConstructible::getCounter() - init;
                REQUIRE(3 == counter.constructorCount());
                REQUIRE(3 == counter.getCopyConstructor());
            }
        }
        WHEN("assign vector")
        {
            Vector v2;
            v2.push_back(pw::test::CopyConstructible(1));
            v2 = v;
            THEN("They are the same")
            {
                REQUIRE(v == v2);
            }
        }
        WHEN("copy construct")
        {
            Vector v2(v);
            THEN("They are the same")
            {
                REQUIRE(v == v2);
            }
        }
    }
}
