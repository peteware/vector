#include <pw/vector>

#include "copyconstructible.h"

#include <stdexcept>
#include <tuple>
#include <vector>

#include "catch2/catch.hpp"

using TestTypeList = std::tuple<pw::vector<CopyConstructible>, std::vector<CopyConstructible>>;
//using TestTypeList = std::tuple<std::vector<CopyConstructible>>;
TEMPLATE_LIST_TEST_CASE("count constructors in vector", "[vector][constructor]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    ConsCounter       counter;
    ConsCounter const init(CopyConstructible::getCounter());

    GIVEN("An empty vector")
    {
        Vector v;

        WHEN("Nothing was called")
        {
            counter = CopyConstructible::getCounter() - init;
            THEN("Nothing was constructed")
            {
                REQUIRE(counter.zero());
            }
        }

        WHEN("reserve() is increased")
        {
            counter = CopyConstructible::getCounter();
            v.reserve(5);
            counter = CopyConstructible::getCounter() - counter;
            THEN("copy constructor not called")
            {
                REQUIRE(counter.zero());
            }
        }
        WHEN("resize() is called")
        {
            size_t const count = 5;

            counter = CopyConstructible::getCounter();
            v.resize(count);
            counter = CopyConstructible::getCounter() - counter;
            THEN("default construted count times")
            {
                REQUIRE(count == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() == counter.constructorCount());
                REQUIRE(counter.allCount() == count);
            }
        }
#if 0
        GIVEN("and an object")
        {
            size_t const      count = 5;
            CopyConstructible copyObject;

            WHEN("push_back() is called")
            {
                init = CopyConstructible::getCounter();
                v.push_back(copyObject);
                THEN("Copy construct called once")
                {
                    counter = CopyConstructible::getCounter() - init;
                    REQUIRE(1 == counter.constructorCount());
                    REQUIRE(counter.constructorCount() == counter.allCount());
                }
            }
            WHEN("push_back(move) is called")
            {
                init = CopyConstructible::getCounter();
                v.push_back(pw::move(copyObject));
                THEN("Copy construct called once")
                {
                    counter = CopyConstructible::getCounter() - init;
                    REQUIRE(1 == counter.getMoveConstructor());
                    REQUIRE(counter.getMoveConstructor() == counter.allCount());
                }
            }
            WHEN("resize(count) is called")
            {
                init = CopyConstructible::getCounter();
                v.resize(count);
                THEN("Copy construct called same amount")
                {
                    counter = CopyConstructible::getCounter() - init;
                    REQUIRE(count == counter.constructorCount());
                    REQUIRE(counter.constructorCount() == counter.allCount());
                }
            }
            WHEN("insert(count) at begin")
            {
                init = CopyConstructible::getCounter();
                v.insert(v.begin(), count, copyObject);
                THEN("Copy constructor called count times")
                {
                    counter = CopyConstructible::getCounter() - init;
                    REQUIRE(count == counter.constructorCount());
                }
            }
        }
        GIVEN("and add count objects")
        {
            size_t const      count = 5;
            CopyConstructible copyObject;
            ConsCounter       startCount(CopyConstructible::getCounter());
            Vector            v(count);
            ConsCounter       init(CopyConstructible::getCounter());

            REQUIRE(v.size() == v.capacity());
            WHEN("getCounter()")
            {
                counter = CopyConstructible::getCounter() - startCount;
                THEN("count items default constructed")
                {
                    REQUIRE(count == counter.getDefaultConstructor());
                    REQUIRE(counter.getDefaultConstructor() == counter.constructorCount());
                }
            }
            WHEN("push_back() is called")
            {
                v.push_back(copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items")
                {
                    REQUIRE(count == counter.getMoveConstructor());
                }
                THEN("Copy constructed new item")
                {
                    REQUIRE(1 == counter.getCopyConstructor());
                }
            }
            WHEN("insert() at begin")
            {
                v.insert(v.begin(), copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items for more space")
                {
                    REQUIRE(count == counter.getMoveConstructor());
                }
                THEN("Assigned item")
                {
                    REQUIRE(1 == counter.getCopyConstructor());
                }
            }
            WHEN("insert() at end")
            {
                v.insert(v.end(), copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items for more space")
                {
                    REQUIRE(count == counter.getMoveConstructor());
                }
                THEN("Copy constructed new item")
                {
                    REQUIRE(1 == counter.getCopyConstructor());
                }
            }
            WHEN("insert() in middle")
            {
                v.insert(v.begin() + count - 2, copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items for more space")
                {
                    REQUIRE(count == counter.getMoveConstructor());
                }
                THEN("Copy constructed new item")
                {
                    REQUIRE(1 == counter.getCopyConstructor());
                }
            }
        }
#endif
    }
    counter = CopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

TEMPLATE_LIST_TEST_CASE("init-list", "[vector][constructor][init-list]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    ConsCounter                       init(CopyConstructible::getCounter());
    ConsCounter                       counter;
    std::initializer_list<value_type> initlist = { 1, 2, 5 };

    counter = CopyConstructible::getCounter() - init;
    REQUIRE(3 == counter.constructorCount());
    REQUIRE(3 == counter.getDefaultConstructor());
    GIVEN("A vector from an init list")
    {
        init = CopyConstructible::getCounter();
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
                counter = CopyConstructible::getCounter() - init;
                REQUIRE(3 == counter.constructorCount());
                REQUIRE(3 == counter.getCopyConstructor());
            }
        }
        WHEN("assign vector")
        {
            Vector v2;
            v2.push_back(CopyConstructible(1));
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
