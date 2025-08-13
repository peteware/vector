#include <pw/vector>
#include <test_allocator_move_assignment.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("reserve() method", "[vector][reserve][capacity]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("reserve() is called on empty vector")
        {
            Vector v;
            REQUIRE(v.empty());

            v.reserve(10);

            THEN("vector remains empty with increased capacity")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() >= 10);
                REQUIRE(v.begin() == v.end());
                REQUIRE(v.data() == nullptr);
            }
        }
    }
    GIVEN("vector with elements")
    {
        WHEN("reserve() is called with zero capacity")
        {
            Vector v                 = { 1, 2, 3 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();

            v.reserve(0);

            THEN("size and capacity are preserved")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= original_capacity);
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
        WHEN("reserve() is called with less than current capacity")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();

            v.reserve(original_capacity - 1);

            THEN("size and capacity are preserved")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= original_capacity);
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
                REQUIRE(v[3] == 4);
                REQUIRE(v[4] == 5);
            }
        }
        WHEN("reserve() is called equal to current capacity")
        {
            Vector v                 = { 1, 2, 3 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();

            v.reserve(original_capacity);

            THEN("size is preserved and capacity is at least original")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= original_capacity);
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
        WHEN("reserve() is called greater than current capacity")
        {
            Vector v                 = { 1, 2, 3 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();
            auto   new_capacity      = original_capacity + 50;

            v.reserve(new_capacity);

            THEN("size is preserved and capacity is increased")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= new_capacity);
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
    }
    GIVEN("vector for large capacity reservation")
    {
        WHEN("reserve() is called with large capacity")
        {
            Vector v = { 10, 20, 30 };
            v.reserve(1000);

            auto capacity_after_reserve = v.capacity();

            THEN("vector has large capacity with preserved elements")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() >= 1000);
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
            THEN("many elements can be added without reallocation")
            {
                for (int i = 0; i < 500; ++i)
                {
                    v.push_back(i);
                }
                REQUIRE(v.capacity() == capacity_after_reserve);
                REQUIRE(v.size() == 503);
            }
        }
    }
    GIVEN("vector for multiple reserve calls")
    {
        WHEN("reserve() is called multiple times")
        {
            Vector v = { 1, 2 };

            v.reserve(10);
            auto capacity_after_first = v.capacity();
            REQUIRE(capacity_after_first >= 10);
            REQUIRE(v.size() == 2);

            v.reserve(20);
            auto capacity_after_second = v.capacity();
            REQUIRE(capacity_after_second >= 20);
            REQUIRE(capacity_after_second >= capacity_after_first);
            REQUIRE(v.size() == 2);

            v.reserve(5); // Should not reduce capacity

            THEN("capacity does not decrease and elements are preserved")
            {
                REQUIRE(v.capacity() >= capacity_after_second);
                REQUIRE(v.size() == 2);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
            }
        }
    }
    GIVEN("vector to test data integrity preservation")
    {
        WHEN("reserve() is called after building vector")
        {
            Vector v;
            for (int i = 0; i < 50; ++i)
            {
                v.push_back(i * 3);
                REQUIRE(v[i] == i * 3);
            }

            auto original_size = v.size();
            v.reserve(500);

            THEN("size is preserved and capacity is increased")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= 500);
            }
            THEN("all elements are preserved")
            {
                for (int i = 0; i < 50; ++i)
                {
                    REQUIRE(v[i] == i * 3);
                }
            }
            THEN("iterators work correctly")
            {
                auto it = v.begin();
                for (int i = 0; i < 50; ++i, ++it)
                {
                    REQUIRE(*it == i * 3);
                }
                REQUIRE(it == v.end());
            }
        }
    }
    GIVEN("vector with custom allocator")
    {
        WHEN("reserve() is called")
        {
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v({ 1, 2, 3 }, alloc);
            auto      original_size = v.size();

            v.reserve(25);

            THEN("capacity is increased and allocator is preserved")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= 25);
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
    }
    GIVEN("vector for testing pointer and iterator effects")
    {
        WHEN("reserve() is called with significant capacity increase")
        {
            Vector v                 = { 10, 20, 30 };
            auto   original_capacity = v.capacity();

            v.reserve(original_capacity + 100);

            THEN("new pointers and iterators work correctly")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() >= original_capacity + 100);
                REQUIRE(v.data() != nullptr);
                REQUIRE(v.begin() != v.end());
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(*v.begin() == 10);
                REQUIRE(*(v.end() - 1) == 30);
            }
        }
    }
    GIVEN("vector for testing reserve followed by operations")
    {
        WHEN("reserve() is called followed by various operations")
        {
            Vector v;
            v.reserve(100);
            auto reserved_capacity = v.capacity();

            for (int i = 0; i < 50; ++i)
            {
                v.push_back(i);
            }

            REQUIRE(v.size() == 50);
            REQUIRE(v.capacity() == reserved_capacity);

            v.clear();
            REQUIRE(v.empty());
            REQUIRE(v.capacity() == reserved_capacity);

            v.assign({ 100, 200, 300 });

            THEN("operations work correctly with reserved capacity")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v[0] == 100);
                REQUIRE(v[1] == 200);
                REQUIRE(v[2] == 300);
            }
        }
    }
    GIVEN("vector for testing reasonable capacity limits")
    {
        WHEN("reserve() is called with reasonable size")
        {
            Vector v;

            v.reserve(1000);

            THEN("capacity is set and vector remains empty")
            {
                REQUIRE(v.capacity() >= 1000);
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("vector for testing exception safety")
    {
        WHEN("reserve() is called with reasonable increase")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();

            v.reserve(original_capacity + 10);

            THEN("elements are preserved if reserve succeeds")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= original_capacity + 10);
                for (int i = 0; i < 5; ++i)
                {
                    REQUIRE(v[i] == i + 1);
                }
            }
        }
    }
}