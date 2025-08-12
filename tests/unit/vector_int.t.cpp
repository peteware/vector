#include "test_allocator_copy_assignment.h"
#include "test_allocator_move_assignment.h"
#include "test_allocator_swapable.h"

#include <pw/vector>
#include <unistd.h>

#include <pw/impl/move.h>

#include <catch2/catch.hpp>

TEST_CASE("Swap uses allocator", "[swap][allocator]")
{
    //
    // constexpr void
    // swap(vector& other) noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
    //                              pw::allocator_traits<allocator_type>::is_always_equal::value);
    //             pw::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
    //             pw::allocator_traits<allocator_type>::is_always_equal::value);
    //     using propagate_on_container_swap            = false_type;
    //     using propagate_on_container_swap            = true_type;
    GIVEN("A vector with propagate_on_container_swap = true")
    {
        using Allocator = pw::test::allocator_swapable<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc1 { 5 };
        Allocator alloc2 { 10 };
        Vector    v1 { alloc1 };
        Vector    v2 { alloc2 };

        REQUIRE(Allocator::propagate_on_container_swap::value);
        REQUIRE(v1.get_allocator() == alloc1);
        WHEN("swap")
        {
            pw::swap(v1, v2);
            THEN("allocators are swapped")
            {
                REQUIRE(v1.get_allocator() == alloc2);
                REQUIRE(v2.get_allocator() == alloc1);
            }
        }
    }
    GIVEN("A vector with propagate_on_container_swap = false")
    {
        using Allocator = pw::test::allocator_base<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc1 { 1 };
        Allocator alloc2 { 2 };
        Vector    v1 { alloc1 };
        Vector    v2 { alloc2 };

        REQUIRE(!Allocator::propagate_on_container_swap::value);
        WHEN("swap")
        {
            pw::swap(v1, v2);
            THEN("allocators are not swapped")
            {
                REQUIRE(v1.get_allocator() == alloc1);
                REQUIRE(v2.get_allocator() == alloc2);
            }
        }
    }
}

TEST_CASE("clear() method", "[vector][clear][modifiers]")
{
    using Vector = pw::vector<int>;

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
                REQUIRE(v.data() == nullptr);
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
                REQUIRE(v.data() == nullptr);
            }
            THEN("capacity is preserved")
            {
                REQUIRE(v.capacity() == original_capacity);
            }
        }
    }
    GIVEN("vector with reserved capacity")
    {
        WHEN("clear() is called after reserving capacity")
        {
            Vector v;
            v.reserve(100);
            auto reserved_capacity = v.capacity();
            REQUIRE(reserved_capacity >= 100);

            v.assign({ 1, 2, 3, 4, 5 });
            REQUIRE(v.size() == 5);

            v.clear();

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("large vector")
    {
        WHEN("clear() is called")
        {
            Vector v(static_cast<Vector::size_type>(1000), 7);
            REQUIRE(v.size() == 1000);
            REQUIRE(!v.empty());
            auto original_capacity = v.capacity();

            v.clear();

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
                REQUIRE(v.data() == nullptr);
            }
            THEN("capacity is preserved")
            {
                REQUIRE(v.capacity() == original_capacity);
            }
        }
    }
    GIVEN("vector that needs to be cleared multiple times")
    {
        WHEN("clear() is called multiple times")
        {
            Vector v                 = { 1, 2, 3 };
            auto   original_capacity = v.capacity();

            v.clear();
            v.clear();
            v.clear();

            THEN("vector remains empty after multiple clears")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() == original_capacity);
            }
        }
    }
    GIVEN("vector for reuse after clearing")
    {
        WHEN("clear() is called then vector is reused")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_capacity = v.capacity();

            v.clear();
            REQUIRE(v.empty());
            REQUIRE(v.capacity() == original_capacity);

            v.assign({ 10, 20 });

            THEN("vector can be reused correctly")
            {
                REQUIRE(v.size() == 2);
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
            }
        }
    }
    GIVEN("vector with custom allocator")
    {
        WHEN("clear() is called")
        {
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v({ 1, 2, 3 }, alloc);
            auto      original_capacity = v.capacity();

            v.clear();

            THEN("vector becomes empty and allocator is preserved")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() == original_capacity);
                REQUIRE(v.get_allocator() == alloc);
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
                REQUIRE(v.data() == nullptr);
                REQUIRE(v.begin() == v.end());
            }
        }
    }
}

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

TEST_CASE("resize() method", "[vector][resize][modifiers]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("resize() is called on empty vector")
        {
            Vector v;
            REQUIRE(v.empty());

            v.resize(5);

            THEN("vector is resized with default values")
            {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 5);
                REQUIRE(!v.empty());

                for (Vector::size_type i = 0; i < 5; ++i)
                {
                    REQUIRE(v[i] == 0);
                }
            }
        }
    }
    GIVEN("non-empty vector")
    {
        WHEN("resize() is called to zero")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_capacity = v.capacity();

            v.resize(0);

            THEN("vector becomes empty with preserved capacity")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() == original_capacity);
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("resize() is called to same size")
        {
            Vector v                 = { 10, 20, 30 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();

            v.resize(original_size);

            THEN("vector remains unchanged")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() == original_capacity);
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
        }
        WHEN("resize() is called to larger size")
        {
            Vector v = { 1, 2, 3 };

            v.resize(7);

            THEN("vector is enlarged with preserved original elements")
            {
                REQUIRE(v.size() == 7);
                REQUIRE(v.capacity() >= 7);
            }
            THEN("original elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
            THEN("new elements are default-constructed")
            {
                REQUIRE(v[3] == 0);
                REQUIRE(v[4] == 0);
                REQUIRE(v[5] == 0);
                REQUIRE(v[6] == 0);
            }
        }
        WHEN("resize() is called to smaller size")
        {
            Vector v                 = { 10, 20, 30, 40, 50 };
            auto   original_capacity = v.capacity();

            v.resize(3);

            THEN("vector is shrunk with preserved capacity")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() == original_capacity);
            }
            THEN("remaining elements are preserved")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
        }
    }
    GIVEN("vector with reserved capacity")
    {
        WHEN("resize() is called within existing capacity")
        {
            Vector v { 1, 2, 3, 4, 5 };
            v.reserve(100);
            auto reserved_capacity = v.capacity();

            v.resize(50);

            THEN("vector is resized within reserved capacity")
            {
                REQUIRE(v.size() == 50);
                REQUIRE(v.capacity() == reserved_capacity);
            }
            THEN("original elements are preserved")
            {
                for (int i = 0; i < 5; ++i)
                {
                    REQUIRE(v[i] == i + 1);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (Vector::size_type i = 5; i < 50; ++i)
                {
                    REQUIRE(v[i] == 0);
                }
            }
        }
        WHEN("resize() is called beyond existing capacity")
        {
            Vector v                 = { 1, 2, 3 };
            auto   original_capacity = v.capacity();

            v.resize(original_capacity + 10);

            THEN("vector is resized with increased capacity")
            {
                REQUIRE(v.size() == original_capacity + 10);
                REQUIRE(v.capacity() > original_capacity);
            }
            THEN("original elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
            THEN("new elements are default-constructed")
            {
                for (Vector::size_type i = 3; i < v.size(); ++i)
                {
                    REQUIRE(v[i] == 0);
                }
            }
        }
    }
    GIVEN("vector for multiple resize operations")
    {
        WHEN("resize() is called multiple times")
        {
            Vector v;

            v.resize(5);
            REQUIRE(v.size() == 5);
            for (Vector::size_type i = 0; i < 5; ++i)
            {
                REQUIRE(v[i] == 0);
            }

            v.resize(3);
            REQUIRE(v.size() == 3);
            for (Vector::size_type i = 0; i < 3; ++i)
            {
                REQUIRE(v[i] == 0);
            }

            v.resize(8);

            THEN("final resize works correctly")
            {
                REQUIRE(v.size() == 8);
                for (Vector::size_type i = 0; i < 8; ++i)
                {
                    REQUIRE(v[i] == 0);
                }
            }
        }
    }
    GIVEN("vector with custom allocator")
    {
        WHEN("resize() is called")
        {
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v({ 1, 2, 3 }, alloc);

            v.resize(6);

            THEN("vector is resized and allocator is preserved")
            {
                REQUIRE(v.size() == 6);
                REQUIRE(v.get_allocator() == alloc);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
                REQUIRE(v[3] == 0);
                REQUIRE(v[4] == 0);
                REQUIRE(v[5] == 0);
            }
            THEN("vector can be resized smaller")
            {
                v.resize(2);
                REQUIRE(v.size() == 2);
                REQUIRE(v.get_allocator() == alloc);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
            }
        }
    }
    GIVEN("vector to test data integrity preservation")
    {
        WHEN("resize() is called to preserve and extend data")
        {
            Vector v;
            for (int i = 0; i < 20; ++i)
            {
                v.push_back(i * 10);
            }

            v.resize(30);

            THEN("vector is enlarged with preserved data")
            {
                REQUIRE(v.size() == 30);
            }
            THEN("original elements are preserved")
            {
                for (int i = 0; i < 20; ++i)
                {
                    REQUIRE(v[i] == i * 10);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (Vector::size_type i = 20; i < 30; ++i)
                {
                    REQUIRE(v[i] == 0);
                }
            }
            THEN("vector can be resized smaller preserving remaining data")
            {
                v.resize(15);
                REQUIRE(v.size() == 15);

                for (int i = 0; i < 15; ++i)
                {
                    REQUIRE(v[i] == i * 10);
                }
            }
        }
    }
    GIVEN("vector for iterator effects testing")
    {
        WHEN("resize() is called affecting iterators")
        {
            Vector v         = { 10, 20, 30 };

            auto   old_begin = v.begin();
            auto   old_end   = v.end();
            REQUIRE(old_end - old_begin == 3);

            v.resize(5);

            auto new_begin = v.begin();
            auto new_end   = v.end();

            THEN("new iterators work correctly")
            {
                REQUIRE(new_end - new_begin == 5);
                REQUIRE(*new_begin == 10);
                REQUIRE(*(new_begin + 1) == 20);
                REQUIRE(*(new_begin + 2) == 30);
                REQUIRE(*(new_begin + 3) == 0);
                REQUIRE(*(new_begin + 4) == 0);
            }
        }
    }
    GIVEN("vector for consistency testing with other methods")
    {
        WHEN("resize() is called and other access methods are used")
        {
            Vector v;

            v.resize(4);
            REQUIRE(v.size() == 4);
            REQUIRE(v.front() == 0);
            REQUIRE(v.back() == 0);
            REQUIRE(v.at(0) == 0);
            REQUIRE(v.at(3) == 0);
            REQUIRE(v[0] == 0);
            REQUIRE(v[3] == 0);

            v[1] = 100;
            v[2] = 200;

            v.resize(6);

            THEN("access methods work consistently")
            {
                REQUIRE(v.size() == 6);
                REQUIRE(v.front() == 0);
                REQUIRE(v.back() == 0);
                REQUIRE(v[1] == 100);
                REQUIRE(v[2] == 200);
                REQUIRE(v[4] == 0);
                REQUIRE(v[5] == 0);
            }
        }
    }
    GIVEN("vector that was cleared")
    {
        WHEN("resize() is called after clear")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_capacity = v.capacity();

            v.clear();
            REQUIRE(v.empty());

            v.resize(3);

            THEN("vector is resized correctly")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() >= original_capacity);
                REQUIRE(v[0] == 0);
                REQUIRE(v[1] == 0);
                REQUIRE(v[2] == 0);
            }
        }
    }
    GIVEN("vector for large size testing")
    {
        WHEN("resize() is called with large size")
        {
            Vector v;

            v.resize(1000);

            THEN("vector is resized to large size")
            {
                REQUIRE(v.size() == 1000);
                REQUIRE(v.capacity() >= 1000);
            }
            THEN("all elements are default-constructed")
            {
                for (Vector::size_type i = 0; i < 1000; ++i)
                {
                    CAPTURE(i);
                    REQUIRE(v[i] == 0);
                }
            }
            THEN("front and back are correct")
            {
                REQUIRE(v.front() == 0);
                REQUIRE(v.back() == 0);
            }
        }
    }
    GIVEN("vector for alternating size testing")
    {
        WHEN("resize() is called alternating between sizes")
        {
            Vector v;

            for (int cycle = 0; cycle < 5; ++cycle)
            {
                v.resize(10);
                REQUIRE(v.size() == 10);
                for (Vector::size_type i = 0; i < 10; ++i)
                {
                    REQUIRE(v[i] == 0);
                }

                v.resize(5);
                REQUIRE(v.size() == 5);
                for (Vector::size_type i = 0; i < 5; ++i)
                {
                    REQUIRE(v[i] == 0);
                }
            }

            THEN("alternating resizes work correctly")
            {
                REQUIRE(v.size() == 5);
            }
        }
    }
}

TEST_CASE("resize() with value method", "[vector][resize][modifiers][value]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("resize() is called with value on empty vector")
        {
            Vector v;

            v.resize(5, 42);

            THEN("vector is resized with specified value")
            {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 5);
                REQUIRE(!v.empty());

                for (Vector::size_type i = 0; i < 5; ++i)
                {
                    REQUIRE(v[i] == 42);
                }
            }
        }
    }
    GIVEN("non-empty vector")
    {
        WHEN("resize() is called to zero with value")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_capacity = v.capacity();

            v.resize(0, 99);

            THEN("vector becomes empty with preserved capacity")
            {
                REQUIRE(v.empty());
                REQUIRE(v.capacity() == original_capacity);
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("resize() is called to same size with value")
        {
            Vector v                 = { 10, 20, 30 };
            auto   original_size     = v.size();
            auto   original_capacity = v.capacity();

            v.resize(original_size, 99);

            THEN("vector remains unchanged (value ignored)")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() == original_capacity);
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
        }
        WHEN("resize() is called to larger size with value")
        {
            Vector v = { 1, 2, 3 };

            v.resize(7, 42);

            THEN("vector is enlarged with preserved original elements")
            {
                REQUIRE(v.size() == 7);
                REQUIRE(v.capacity() >= 7);
            }
            THEN("original elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
            THEN("new elements have specified value")
            {
                REQUIRE(v[3] == 42);
                REQUIRE(v[4] == 42);
                REQUIRE(v[5] == 42);
                REQUIRE(v[6] == 42);
            }
        }
        WHEN("resize() is called to smaller size with value")
        {
            Vector v                 = { 10, 20, 30, 40, 50 };
            auto   original_capacity = v.capacity();

            v.resize(3, 99);

            THEN("vector is shrunk with preserved capacity")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() == original_capacity);
            }
            THEN("remaining elements are preserved (value ignored when shrinking)")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
        }
    }
    GIVEN("vector with reserved capacity")
    {
        WHEN("resize() is called within existing capacity with value")
        {
            Vector v { 1, 2, 3, 4, 5 };
            v.reserve(100);
            auto reserved_capacity = v.capacity();

            v.resize(50, 77);

            THEN("vector is resized within reserved capacity")
            {
                REQUIRE(v.size() == 50);
                REQUIRE(v.capacity() == reserved_capacity);
            }
            THEN("original elements are preserved")
            {
                for (int i = 0; i < 5; ++i)
                {
                    REQUIRE(v[i] == i + 1);
                }
            }
            THEN("new elements have specified value")
            {
                for (Vector::size_type i = 5; i < 50; ++i)
                {
                    REQUIRE(v[i] == 77);
                }
            }
        }
        WHEN("resize() is called beyond existing capacity with value")
        {
            Vector v                 = { 1, 2, 3 };
            auto   original_capacity = v.capacity();

            v.resize(original_capacity + 10, 88);

            THEN("vector is resized with increased capacity")
            {
                REQUIRE(v.size() == original_capacity + 10);
                REQUIRE(v.capacity() > original_capacity);
            }
            THEN("original elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
            THEN("new elements have specified value")
            {
                for (Vector::size_type i = 3; i < v.size(); ++i)
                {
                    REQUIRE(v[i] == 88);
                }
            }
        }
    }
    GIVEN("vector for multiple resize operations with different values")
    {
        WHEN("resize() is called multiple times with different values")
        {
            Vector v;

            v.resize(3, 10);
            REQUIRE(v.size() == 3);
            for (Vector::size_type i = 0; i < 3; ++i)
            {
                REQUIRE(v[i] == 10);
            }

            v.resize(6, 20);
            REQUIRE(v.size() == 6);
            for (Vector::size_type i = 0; i < 3; ++i)
            {
                REQUIRE(v[i] == 10);
            }
            for (Vector::size_type i = 3; i < 6; ++i)
            {
                REQUIRE(v[i] == 20);
            }

            v.resize(9, 30);

            THEN("all values are correct after multiple resizes")
            {
                REQUIRE(v.size() == 9);
                for (Vector::size_type i = 0; i < 3; ++i)
                {
                    REQUIRE(v[i] == 10);
                }
                for (Vector::size_type i = 3; i < 6; ++i)
                {
                    REQUIRE(v[i] == 20);
                }
                for (Vector::size_type i = 6; i < 9; ++i)
                {
                    REQUIRE(v[i] == 30);
                }
            }
        }
    }
    GIVEN("vector for testing special values")
    {
        WHEN("resize() is called with negative values")
        {
            Vector v = { 1, 2 };

            v.resize(5, -42);

            THEN("negative values are correctly assigned")
            {
                REQUIRE(v.size() == 5);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == -42);
                REQUIRE(v[3] == -42);
                REQUIRE(v[4] == -42);
            }
        }
        WHEN("resize() is called with zero value")
        {
            Vector v = { 1, 2, 3 };

            v.resize(6, 0);

            THEN("zero values are correctly assigned")
            {
                REQUIRE(v.size() == 6);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
                REQUIRE(v[3] == 0);
                REQUIRE(v[4] == 0);
                REQUIRE(v[5] == 0);
            }
        }
    }
    GIVEN("vector with custom allocator")
    {
        WHEN("resize() is called with value")
        {
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v({ 1, 2, 3 }, alloc);

            v.resize(6, 99);

            THEN("vector is resized and allocator is preserved")
            {
                REQUIRE(v.size() == 6);
                REQUIRE(v.get_allocator() == alloc);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
                REQUIRE(v[3] == 99);
                REQUIRE(v[4] == 99);
                REQUIRE(v[5] == 99);
            }
            THEN("vector can be resized smaller")
            {
                v.resize(2, 77);
                REQUIRE(v.size() == 2);
                REQUIRE(v.get_allocator() == alloc);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
            }
        }
    }
    GIVEN("vector for large size testing with value")
    {
        WHEN("resize() is called with large size and value")
        {
            Vector v;

            v.resize(1000, 123);

            THEN("vector is resized to large size with specified value")
            {
                REQUIRE(v.size() == 1000);
                REQUIRE(v.capacity() >= 1000);
            }
            THEN("all elements have the specified value")
            {
                for (Vector::size_type i = 0; i < 1000; ++i)
                {
                    REQUIRE(v[i] == 123);
                }
            }
            THEN("front and back have specified value")
            {
                REQUIRE(v.front() == 123);
                REQUIRE(v.back() == 123);
            }
        }
    }
    GIVEN("vector for testing extreme values")
    {
        WHEN("resize() is called with extreme integer values")
        {
            Vector v;

            v.resize(3, 2147483647); // max int
            REQUIRE(v.size() == 3);
            for (Vector::size_type i = 0; i < 3; ++i)
            {
                REQUIRE(v[i] == 2147483647);
            }

            v.resize(6, -2147483648); // min int

            THEN("extreme values are handled correctly")
            {
                REQUIRE(v.size() == 6);
                for (Vector::size_type i = 0; i < 3; ++i)
                {
                    REQUIRE(v[i] == 2147483647);
                }
                for (Vector::size_type i = 3; i < 6; ++i)
                {
                    REQUIRE(v[i] == -2147483648);
                }
            }
        }
    }
    GIVEN("vector for consistency testing with other methods")
    {
        WHEN("resize() is called with value and other access methods are used")
        {
            Vector v;

            v.resize(4, 55);
            REQUIRE(v.size() == 4);
            REQUIRE(v.front() == 55);
            REQUIRE(v.back() == 55);
            REQUIRE(v.at(0) == 55);
            REQUIRE(v.at(3) == 55);
            REQUIRE(v[0] == 55);
            REQUIRE(v[3] == 55);

            v[1] = 100;
            v[2] = 200;

            v.resize(6, 66);

            THEN("access methods work consistently with mixed values")
            {
                REQUIRE(v.size() == 6);
                REQUIRE(v.front() == 55);
                REQUIRE(v.back() == 66);
                REQUIRE(v[0] == 55);
                REQUIRE(v[1] == 100);
                REQUIRE(v[2] == 200);
                REQUIRE(v[3] == 55);
                REQUIRE(v[4] == 66);
                REQUIRE(v[5] == 66);
            }
        }
    }
    GIVEN("vector that was cleared")
    {
        WHEN("resize() is called with value after clear")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_capacity = v.capacity();

            v.clear();
            REQUIRE(v.empty());

            v.resize(3, 111);

            THEN("vector is resized correctly with specified value")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.capacity() >= original_capacity);
                REQUIRE(v[0] == 111);
                REQUIRE(v[1] == 111);
                REQUIRE(v[2] == 111);
            }
        }
    }
    GIVEN("vector for iterator effects testing with value")
    {
        WHEN("resize() is called with value affecting iterators")
        {
            Vector v         = { 10, 20, 30 };

            auto   old_begin = v.begin();
            auto   old_end   = v.end();
            REQUIRE(old_end - old_begin == 3);

            v.resize(5, 44);

            auto new_begin = v.begin();
            auto new_end   = v.end();

            THEN("new iterators work correctly with specified value")
            {
                REQUIRE(new_end - new_begin == 5);
                REQUIRE(*new_begin == 10);
                REQUIRE(*(new_begin + 1) == 20);
                REQUIRE(*(new_begin + 2) == 30);
                REQUIRE(*(new_begin + 3) == 44);
                REQUIRE(*(new_begin + 4) == 44);
            }
        }
    }
    GIVEN("vector for mixed operations testing")
    {
        WHEN("resize() is called with value mixed with other operations")
        {
            Vector v;

            v.resize(3, 10);
            REQUIRE(v.size() == 3);

            v.push_back(15);
            REQUIRE(v.size() == 4);
            REQUIRE(v[3] == 15);

            v.resize(7, 20);
            REQUIRE(v.size() == 7);
            REQUIRE(v[0] == 10);
            REQUIRE(v[1] == 10);
            REQUIRE(v[2] == 10);
            REQUIRE(v[3] == 15);
            REQUIRE(v[4] == 20);
            REQUIRE(v[5] == 20);
            REQUIRE(v[6] == 20);

            v.resize(2, 99);

            THEN("mixed operations work correctly")
            {
                REQUIRE(v.size() == 2);
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 10);
            }
        }
    }
}
