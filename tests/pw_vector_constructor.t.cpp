#include <pw/vector>

#include "copyconstructible.h"

#include <stdexcept>
#include <tuple>

#include "catch2/catch.hpp"
//using TestTypeList = std::tuple<int, std::string, std::vector<int>>;
//using TestTypeList = std::tuple<int, std::string, float>;
TEST_CASE("count constructors in vector", "[vector][constructor]")
{
    using Vector = pw::vector<CopyConstructible>;

    CopyConstructible copyObject;
    ConsCounter       init(CopyConstructible::getCounter());
    ConsCounter       counter;
    Vector            v;

    GIVEN("An empty vector")
    {
        WHEN("getCounter()")
        {
            counter = CopyConstructible::getCounter() - init;
            THEN("nothing was called")
            {
                REQUIRE(counter.zero());
            }
        }
        WHEN("reserve() is increased")
        {
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
            v.resize(count);
            counter = CopyConstructible::getCounter() - init;
            THEN("default construted count times")
            {
                REQUIRE(count == counter.getDefault());
                REQUIRE(counter.getDefault() == counter.constructorCount());
                REQUIRE(counter.allCount() == count);
            }
        }
        WHEN("push_back() is called")
        {
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
            v.push_back(pw::move(copyObject));
            THEN("Copy construct called once")
            {
                counter = CopyConstructible::getCounter() - init;
                REQUIRE(1 == counter.getMove());
                REQUIRE(counter.getMove() == counter.allCount());
            }
        }
        WHEN("resize() is called")
        {
            v.resize(5);
            THEN("Copy construct called same amount")
            {
                counter = CopyConstructible::getCounter() - init;
                REQUIRE(5 == counter.constructorCount());
                REQUIRE(counter.constructorCount() == counter.allCount());
            }
        }
        WHEN("insert(5) at begin")
        {
            v.insert(v.begin(), 5, copyObject);
            THEN("Copy constructor called 5 times")
            {
                counter = CopyConstructible::getCounter() - init;
                REQUIRE(5 == counter.constructorCount());
            }
        }
    }
}
