#include <pw/vector>
#include <pw/type_traits>
#include <catch2/catch.hpp>

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
                REQUIRE(v.empty());
                REQUIRE(v.size() == 0);
                REQUIRE(v.capacity() == 0);
            }
        }

        WHEN("One item is push_back()'d")
        {
            v.push_back(TestType());

            THEN("Size has changed")
            {
                REQUIRE(!v.empty());
                REQUIRE(v.size() == static_cast<typename pw::vector<TestType>::size_type>(1));
                REQUIRE(v.capacity() >= v.size());
            }
        }
    }
}
