#include <test_testtype.h>
#include <test_values.h>

#include <pw/algorithm>

#include <catch2/catch.hpp>

/*
 * Type requirements:
 * - empty(): no extra
 * - size(): no extra
 * - max_size(): no extra
 * - reserve(): MoveInsertable
 * - capacity(): no extra
 * - shrink_to_fit(): MoveInsertable
 *
 * Exceptions:
 * - reserve(): yes
 */
TEMPLATE_LIST_TEST_CASE("capacity methods", "[vector][capacity]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("empty() is called")
        {
            THEN("it is true")
            {
                REQUIRE(v.empty());
            }
        }
        WHEN("size() is called")
        {
            THEN("it is 0")
            {
                REQUIRE(0 == v.size());
            }
        }
        WHEN("max_size() is called")
        {
            THEN("it is bigger then 0")
            {
                REQUIRE(10000 < v.max_size());
            }
        }
        WHEN("capacity() is called")
        {
            THEN("it is 0")
            {
                REQUIRE(0 == v.capacity());
            }
        }
        GIVEN("and it is const")
        {
            Vector const& c = v;
            REQUIRE(c.empty());
            REQUIRE(c.size() == 0);
            REQUIRE(c.capacity() == 0);
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

        WHEN("empty() is called")
        {
            bool e = v.empty();
            THEN("it is not empty")
            {
                REQUIRE(!e);
            }
        }
        WHEN("size() is called")
        {
            size_t s = v.size();
            THEN("it is the same as count")
            {
                REQUIRE(generate.count == s);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("capacity is same")
            {
                REQUIRE(generate.count == c);
            }
        }
        WHEN("empty() is called")
        {
            bool e = v.empty();
            THEN("it is not empty")
            {
                REQUIRE(!e);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("it is at least 1)")
            {
                REQUIRE(c >= generate.count);
            }
        }
        WHEN("reserve() is called")
        {
            size_t capacity    = v.capacity();
            size_t newCapacity = capacity + 3;
            v.reserve(newCapacity);
            THEN("capacity increases")
            {
                REQUIRE(newCapacity == v.capacity());
            }
        }
        WHEN("reserve() is called with current capacity")
        {
            size_t capacity = v.capacity();
            v.reserve(capacity);
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
        }
        GIVEN("And extra capacity")
        {
            size_t const total = generate.count + 10;
            v.reserve(total);
            REQUIRE(total == v.capacity());
        }
    }
}
