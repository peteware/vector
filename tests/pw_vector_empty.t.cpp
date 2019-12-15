#include <pw/vector>
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
    pw::vector<TestType> v;

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}
