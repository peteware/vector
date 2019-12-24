#include "catch2/catch.hpp"
#include <pw/allocator>
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
    using Vector = pw::vector<TestType>;
    Vector v;
    GIVEN("An empty vector of TestType")
    {
        REQUIRE(pw::is_same<TestType*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<TestType, typename Vector::value_type>::value);
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);

        WHEN("get_allocator() const is called")
        {
            typename Vector::allocator_type a = v.get_allocator();
            THEN("it returns same allocator") { REQUIRE(a == pw::internal::allocator<TestType>()); }
        }
        WHEN("at() is called")
        {
            THEN("at(0) fails") { CHECK_THROWS_AS(v.at(0), std::out_of_range); }
            THEN("at(10) fails") { CHECK_THROWS_AS(v.at(10), std::out_of_range); }
        }
    }
    GIVEN("An empty const vector of TestType")
    {
        Vector const& c = v;
        REQUIRE(c.empty());
        REQUIRE(c.size() == 0);
        REQUIRE(c.capacity() == 0);

        WHEN("get_allocator() const is called")
        {
            typename Vector::allocator_type a = v.get_allocator();
            THEN("it returns same allocator") { REQUIRE(a == pw::internal::allocator<TestType>()); }
        }

        WHEN("at() const is called")
        {
            THEN("at(0) const fails") { CHECK_THROWS_AS(c.at(0), std::out_of_range); }
            THEN("at(10) const fails") { CHECK_THROWS_AS(c.at(10), std::out_of_range); }
        }
    }
    GIVEN("A vector of TestType with 1 item")
    {
        v.push_back(TestType());
        WHEN("empty() is called")
        {
            bool e = v.empty();
            THEN("it is not empty") { REQUIRE(!e); }
        }
        WHEN("size() is called")
        {
            size_t s = v.size();
            THEN("size is 1") { REQUIRE(s == 1); }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("it is at least 1)") { REQUIRE(c >= 1); }
        }
        WHEN("at(0) is called")
        {
            TestType& r = v.at(0);
            THEN("it works") { REQUIRE(r == TestType()); }
        }
        WHEN("at(1) is called")
        {
            THEN("it raises exception") { CHECK_THROWS_AS(v.at(1), std::out_of_range); }
        }
        WHEN("front() is called")
        {
            TestType& r = v.front();
            THEN("it works") { REQUIRE(r == TestType()); }
        }
        WHEN("back() is called")
        {
            TestType& r = v.back();
            THEN("it works") { REQUIRE(r == TestType()); }
        }
        WHEN("data() is called")
        {
            TestType* p = v.data();
            THEN("it works") { REQUIRE(*p == TestType()); }
        }
        WHEN("reserve() is called")
        {
            size_t capacity    = v.capacity();
            size_t newCapacity = capacity + 3;
            v.reserve(newCapacity);
            THEN("capacity increases") { REQUIRE(newCapacity == v.capacity()); }
        }
        WHEN("reserve() is called with current capacity")
        {
            size_t capacity = v.capacity();
            v.reserve(capacity);
            THEN("capacity is unchanged") { REQUIRE(capacity == v.capacity()); }
        }
    }
    GIVEN("A const vector of TestType with 1 item")
    {
        v.push_back(TestType());
        Vector const& c = v;

        WHEN("at(0) const is called")
        {
            TestType const& r = c.at(0);
            THEN("it works") { REQUIRE(r == TestType()); }
        }
        WHEN("at(1) const is called")
        {
            THEN("it raises exception") { CHECK_THROWS_AS(c.at(1), std::out_of_range); }
        }
        WHEN("front() const is called")
        {
            TestType const& r = v.front();
            THEN("it works") { REQUIRE(r == TestType()); }
        }
        WHEN("back() const is called")
        {
            TestType const& r = v.back();
            THEN("it works") { REQUIRE(r == TestType()); }
        }
        WHEN("data() const is called")
        {
            TestType const* p = v.data();
            THEN("it works") { REQUIRE(*p == TestType()); }
        }
    }
}
