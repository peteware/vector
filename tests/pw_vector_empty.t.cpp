#include "catch2/catch.hpp"
#include <pw/type_traits>
#include <pw/vector>

#include <stdexcept>
#include <string>

// SCENARIO( "vectors can be sized and resized", "[vector]" ) {

//     GIVEN( "A vector with some items" ) {
//         std::vector<int> v( 5 );

//         REQUIRE( v.size() == 5 );
//         REQUIRE( v.capacity() >= 5 );
//         WHEN( "the size is increased" ) {
//             v.resize( 10 );

//             THEN( "the size and capacity change" ) {
//                 REQUIRE( v.size() == 10 );
//                 REQUIRE( v.capacity() >= 10 );
//             }
//         }
//     }
// }

TEMPLATE_TEST_CASE("empty vectors work", "[vector][template]", int, std::string)
{
    GIVEN("An empty vector of TestType")
    {
        pw::vector<TestType> v;
        WHEN("nothing changes")
        {
            THEN("Check the types are expected")
            {
                REQUIRE(pw::is_same<TestType*, typename pw::vector<TestType>::pointer>::value);
                REQUIRE(pw::is_same<TestType, typename pw::vector<TestType>::value_type>::value);
            }
            THEN("It is reported as empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.size() == 0);
                REQUIRE(v.capacity() == 0);
            }
            THEN("at() fails")
            {
                THEN("at(0) fails") { CHECK_THROWS_AS(v.at(0), std::out_of_range); }
                THEN("at(10) fails") { CHECK_THROWS_AS(v.at(10), std::out_of_range); }
            }
        }
    }
    GIVEN("A vector of TestType with 1 item")
    {
        pw::vector<TestType> v;
        v.push_back(TestType());
        WHEN("size is checked")
        {
            size_t s = v.size();
            THEN("size is 1") { REQUIRE(s == 1); }
        }
        WHEN("empty is checked")
        {
            bool e = v.empty();
            THEN("it is not empty") { REQUIRE(!e); }
        }
        WHEN("capacity is checked")
        {
            size_t c = v.capacity();
            THEN("it is at least 1)") { REQUIRE(c >= 1); }
        }
        WHEN("at(0) is called")
        {
            THEN("it works")
            {
                TestType& r = v.at(0);
                REQUIRE(r == TestType());
            }
        }
        WHEN("at(1) is called")
        {
            THEN("it raises exception") { CHECK_THROWS_AS(v.at(1), std::out_of_range); }
        }
    }
}
