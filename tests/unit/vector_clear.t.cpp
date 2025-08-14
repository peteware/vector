#include <pw/algorithm>
#include <pw/vector>
#include <test_allocator_move_assignment.h>
#include <test_testtype.h>
#include <test_values.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

/*
 * Type requirements
 * - clear(): No extra
 */
TEMPLATE_LIST_TEST_CASE("clear methods", "[vector][clear]", pw::test::TestTypeList)
{
    using Vector = TestType;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("clear() is called")
        {
            v.clear();
            THEN("vector is empty")
            {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

        WHEN("clear() is called")
        {
            v.clear();
            THEN("it is empty")
            {
                REQUIRE(v.empty());
            }
            THEN("capacity() is same")
            {
                REQUIRE(generate.count == v.capacity());
            }
        }
        WHEN("clear() is called then shrink_to_fit()")
        {
            v.clear();
            v.shrink_to_fit();
            THEN("capacity() is 0")
            {
                REQUIRE(0 == v.capacity());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("clear() method", "[vector][clear][modifiers]", pw::test::TestTypeListInt)
{
    using Vector = TestType;

    GIVEN("empty vector")
    {
        WHEN("clear() is called on empty vector")
        {
            Vector v;
            REQUIRE(v.empty());

            v.clear();

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
                REQUIRE(v.data() == nullptr);
            }
        }
    }
    GIVEN("single element vector")
    {
        WHEN("clear() is called")
        {
            Vector v = { 42 };
            REQUIRE(!v.empty());
            REQUIRE(v.size() == 1);
            auto original_capacity = v.capacity();

            v.clear();

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
            THEN("capacity is preserved")
            {
                REQUIRE(v.capacity() == original_capacity);
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("clear() is called")
        {
            Vector v = { 10, 20, 30, 40, 50 };
            REQUIRE(!v.empty());
            REQUIRE(v.size() == 5);
            auto original_capacity = v.capacity();

            v.clear();

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
            THEN("capacity is preserved")
            {
                REQUIRE(v.capacity() == original_capacity);
            }
        }
    }
    GIVEN("vector with reserved capacity")
    {
        Vector v { 1, 2, 3 };
        v.reserve(100);
        WHEN("clear() is called after reserving capacity")
        {
            v.clear();
            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("vector for reuse after clearing")
    {
        Vector v = { 1, 2, 3, 4, 5 };
        WHEN("clear() is called and assign()'d")
        {
            v.clear();
            v.assign({ 10, 20 });
            THEN("vector has expected elements")
            {
                REQUIRE(v.size() == 2);
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
            }
        }
    }
    GIVEN("vector with iterators")
    {
        WHEN("clear() is called")
        {
            Vector v            = { 1, 2, 3, 4 };

            auto   begin_before = v.begin();
            auto   end_before   = v.end();
            REQUIRE(begin_before != end_before);

            v.clear();

            auto begin_after = v.begin();
            auto end_after   = v.end();

            THEN("new iterators are equal (empty range)")
            {
                REQUIRE(begin_after == end_after);
            }
        }
    }
    GIVEN("vector to be cleared and then accessed")
    {
        WHEN("clear() is called followed by access methods")
        {
            Vector v = { 1, 2, 3 };

            v.clear();
            REQUIRE(v.empty());

            THEN("access methods behave correctly for empty vector")
            {
                REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
                REQUIRE(v.begin() == v.end());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("vector_clear", "[vector_clear]", pw::test::TestTypeListAllocatorBase)
{
    using Vector         = TestType;
    using allocator_type = Vector::allocator_type;
    GIVEN("vector with custom allocator")
    {
        WHEN("clear() is called")
        {
            allocator_type alloc(5);
            Vector         v({ 1, 2, 3 }, alloc);

            v.clear();
            THEN("vector is empty and allocator is preserved")
            {
                REQUIRE(v.empty());
                REQUIRE(v.get_allocator() == alloc);
            }
        }
    }
}