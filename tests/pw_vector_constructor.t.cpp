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

    GIVEN("An empty vector")
    {
        ConsCounter init(CopyConstructible::getCounter());
        ConsCounter count;
        Vector      v;

        count = CopyConstructible::getCounter() - init;
        REQUIRE(count.zero());

        WHEN("reserve() is increased")
        {
            init = CopyConstructible::getCounter();
            v.reserve(5);
            count = CopyConstructible::getCounter() - init;
            THEN("copy constructor not called")
            {
                REQUIRE(count.zero());
            }
        }

        WHEN("push_back() is called")
        {
            CopyConstructible c;
            init = CopyConstructible::getCounter();
            v.push_back(c);
            THEN("Copy construct called once")
            {
                count = CopyConstructible::getCounter() - init;
                REQUIRE(1 == count.constructorCount());
            }
        }
        WHEN("resize() is called")
        {
            init = CopyConstructible::getCounter();
            v.resize(5);
            THEN("Copy construct called same amount")
            {
                count = CopyConstructible::getCounter() - init;
                REQUIRE(5 == count.constructorCount());
            }
        }
    }
    GIVEN("A vector with elements and extra space")
    {
        ConsCounter init(CopyConstructible::getCounter());
        ConsCounter count;
        Vector      v(5);
        v.reserve(10);
        REQUIRE(10 == v.capacity());
        WHEN("shrink_to_fit() is called")
        {
            v.shrink_to_fit();
            THEN("size() and capacity() are same")
            {
                REQUIRE(v.size() == v.capacity());
            }
        }

        WHEN("clear() is called then shrink_to_fit()")
        {
            v.clear();
            v.shrink_to_fit();
            THEN("size() is 0")
            {
                REQUIRE(0 == v.size());
            }
            THEN("capacity() is 0")
            {
                REQUIRE(0 == v.capacity());
            }
        }
    }
}
