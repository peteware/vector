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

    ConsCounter counter;
    ConsCounter init(CopyConstructible::getCounter());

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
            init = CopyConstructible::getCounter();
            v.reserve(5);
            counter = CopyConstructible::getCounter() - init;
            THEN("copy constructor not called")
            {
                REQUIRE(counter.zero());
            }
        }
        WHEN("resize() is called")
        {
            size_t const count = 5;

            init = CopyConstructible::getCounter();
            v.resize(count);
            counter = CopyConstructible::getCounter() - init;
            THEN("default construted count times")
            {
                REQUIRE(count == counter.getDefault());
                REQUIRE(counter.getDefault() == counter.constructorCount());
                REQUIRE(counter.allCount() == count);
            }
        }
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
                    REQUIRE(1 == counter.getMove());
                    REQUIRE(counter.getMove() == counter.allCount());
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
                    REQUIRE(count == counter.getDefault());
                    REQUIRE(counter.getDefault() == counter.constructorCount());
                }
            }
            WHEN("push_back() is called")
            {
                v.push_back(copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items")
                {
                    REQUIRE(count == counter.getMove());
                }
                THEN("Copy constructed new item")
                {
                    REQUIRE(1 == counter.getCopy());
                }
            }
            WHEN("insert() at begin")
            {
                v.insert(v.begin(), copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items for more space")
                {
                    REQUIRE(count == counter.getMove());
                }
                THEN("Assigned item")
                {
                    REQUIRE(1 == counter.getCopy());
                }
            }
            WHEN("insert() at end")
            {
                v.insert(v.end(), copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items for more space")
                {
                    REQUIRE(count == counter.getMove());
                }
                THEN("Copy constructed new item")
                {
                    REQUIRE(1 == counter.getCopy());
                }
            }
            WHEN("insert() in middle")
            {
                v.insert(v.begin() + count - 2, copyObject);
                counter = CopyConstructible::getCounter() - init;
                THEN("Move constructed existing items for more space")
                {
                    REQUIRE(count == counter.getMove());
                }
                THEN("Copy constructed new item")
                {
                    REQUIRE(1 == counter.getCopy());
                }
            }
        }
    }
    counter = CopyConstructible::getCounter();
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}
