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

        count = CopyConstructible::getCounter() - count;
        REQUIRE(count.zero());

        WHEN("reserve() is increased")
        {
            init = CopyConstructible::getCounter() - count;
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
            init = CopyConstructible::getCounter() - count;
            v.push_back(c);
            count = CopyConstructible::getCounter() - init;
            THEN("Copy construct called once")
            {
                REQUIRE(1 == count.constructorCount());
            }
        }
    }
}
