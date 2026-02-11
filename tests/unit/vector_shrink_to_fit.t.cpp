#include <pw/vector>
#include <test_allocator_move_assignment.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("shrink_to_fit() method", "[vector][shrink_to_fit][capacity]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("shrink_to_fit() is called on empty vector")
        {
            Vector v;
            REQUIRE(v.empty());
            auto original_capacity = v.capacity();

            v.shrink_to_fit();
            THEN("vector remains empty with possibly reduced capacity")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() <= original_capacity);
                REQUIRE(v.begin() == v.end());
            }
        }
    }
    GIVEN("vector with elements")
    {
        WHEN("shrink_to_fit() is called when capacity equals size")
        {
            Vector v             = { 1, 2, 3 };
            auto   original_size = v.size();

            v.shrink_to_fit();
            THEN("size is preserved and capacity is adequate")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
    }
    GIVEN("vector after reserve")
    {
        WHEN("shrink_to_fit() is called after reserve")
        {
            Vector v = { 1, 2, 3, 4, 5 };
            v.reserve(100);
            auto size_before     = v.size();
            auto capacity_before = v.capacity();
            REQUIRE(capacity_before >= 100);
            REQUIRE(capacity_before > size_before);

            v.shrink_to_fit();

            THEN("size is preserved and capacity is potentially reduced")
            {
                REQUIRE(v.size() == size_before);
                REQUIRE(v.capacity() <= capacity_before);
                REQUIRE(v.capacity() >= v.size());
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
    }
    GIVEN("vector that was cleared after having capacity")
    {
        WHEN("shrink_to_fit() is called after clear")
        {
            Vector v;
            v.reserve(50);
            v.assign({ 1, 2, 3 });
            v.clear();

            auto capacity_before = v.capacity();
            REQUIRE(v.empty());
            REQUIRE(capacity_before > 0);

            v.shrink_to_fit();

            THEN("vector remains empty with potentially reduced capacity")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() <= capacity_before);
            }
        }
    }
    GIVEN("vector for multiple shrink_to_fit calls")
    {
        WHEN("shrink_to_fit() is called multiple times")
        {
            Vector v;
            v.reserve(100);
            v.assign({ 1, 2, 3 });

            v.shrink_to_fit();
            auto capacity_after_first = v.capacity();
            REQUIRE(v.size() == 3);
            REQUIRE(capacity_after_first >= 3);

            v.shrink_to_fit();
            auto capacity_after_second = v.capacity();

            THEN("capacity stabilizes after multiple calls")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(capacity_after_second == capacity_after_first);
            }
            THEN("elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
    }
    GIVEN("large vector with many elements")
    {
        WHEN("shrink_to_fit() is called to preserve data integrity")
        {
            Vector v;
            v.reserve(1000);
            for (int i = 0; i < 100; ++i)
            {
                v.push_back(i * 2);
            }

            auto capacity_before = v.capacity();
            REQUIRE(capacity_before >= 1000);
            REQUIRE(v.size() == 100);

            v.shrink_to_fit();

            THEN("size and capacity are correct")
            {
                REQUIRE(v.size() == 100);
                REQUIRE(v.capacity() <= capacity_before);
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("all elements are preserved and correct")
            {
                for (int i = 0; i < 100; ++i)
                {
                    REQUIRE(v[i] == i * 2);
                }
            }
            THEN("iterators work correctly")
            {
                auto it = v.begin();
                for (int i = 0; i < 100; ++i, ++it)
                {
                    REQUIRE(*it == i * 2);
                }
                REQUIRE(it == v.end());
            }
        }
    }
    GIVEN("vector with reserved capacity for iterator effects")
    {
        WHEN("shrink_to_fit() is called affecting pointers and iterators")
        {
            Vector v;
            v.reserve(100);
            v.assign({ 10, 20, 30 });

            v.shrink_to_fit();

            THEN("new pointers and iterators work correctly")
            {
                REQUIRE(v.size() == 3);
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
    GIVEN("vector with custom allocator")
    {
        WHEN("shrink_to_fit() is called")
        {
            // TODO: Seems wrong place to use test::allocator_move_assignment
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v(alloc);
            v.assign({ 1, 2, 3 });
            v.reserve(50);

            auto capacity_before = v.capacity();
            REQUIRE(capacity_before >= 50);

            v.shrink_to_fit();

            THEN("capacity is reduced and allocator is preserved")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() <= capacity_before);
                REQUIRE(v.capacity() >= v.size());
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
    GIVEN("vector to test non-binding request behavior")
    {
        WHEN("shrink_to_fit() is called as non-binding request")
        {
            Vector v = { 1, 2, 3 };

            v.shrink_to_fit();

            THEN("capacity is reasonable and elements are preserved")
            {
                REQUIRE(v.capacity() >= v.size());
                REQUIRE(v.size() == 3);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
    }
}