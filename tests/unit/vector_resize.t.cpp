#include <test_allocator_move_assignment.h>
#include <test_copyconstructible.h>
#include <test_optracker_defaultcopyconstructible.h>
#include <test_permute.h>
#include <test_testtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

/*
 * Type requirements
 * - resize(size_type count): MoveInsertable and DefaultInsertable
 * - resize(size_type count, const value_type& value): CopyInsertable
 */
TEMPLATE_LIST_TEST_CASE("Test resize()", "[vector][resize]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("resize() is 0")
        {
            constexpr size_t size = 0;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
                REQUIRE(v.empty());
            }
        }
        WHEN("resize() increases size")
        {
            constexpr size_t size = 3;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
                REQUIRE(!v.empty());
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
        WHEN("resize() to large size")
        {
            constexpr size_t size = 1000;

            v.resize(size);
            THEN("size is changed correctly")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
            THEN("all elements are default-constructed")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        constexpr size_t count = 5;
        value_type       first_value;
        value_type       value;
        Vector           values;

        for (size_t i = 0; i < count; ++i)
        {
            pw::test::permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
        }
        Vector v(values);

        WHEN("resize() is 0")
        {
            constexpr size_t size = 0;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
                REQUIRE(v.empty());
            }
        }
        WHEN("resize() increases size")
        {
            constexpr size_t size = count + 3;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
            THEN("original elements are preserved")
            {
                for (size_t i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == values[i]);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = count; i < size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
        WHEN("resize() decreases size")
        {
            size_t const     capacity = v.capacity();
            constexpr size_t size     = count - 2;

            v.resize(size);
            THEN("size is smaller")
            {
                REQUIRE(size == v.size());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
            THEN("remaining elements are preserved")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == values[i]);
                }
            }
        }
        WHEN("resize() decreases to 1 element")
        {
            size_t const     capacity = v.capacity();
            constexpr size_t size     = 1;

            v.resize(size);
            THEN("size is 1")
            {
                REQUIRE(size == v.size());
                REQUIRE(!v.empty());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
            THEN("first element is preserved")
            {
                REQUIRE(v[0] == first_value);
            }
        }
        WHEN("resize() does not change size")
        {
            size_t const     capacity = v.capacity();
            constexpr size_t size     = count;

            v.resize(size);
            THEN("size is unchanged")
            {
                REQUIRE(size == v.size());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
            THEN("all elements are preserved")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == values[i]);
                }
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Test resize() edge cases", "[vector][resize][edge]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("A vector at maximum capacity")
    {
        Vector v;
        v.reserve(10);
        for (size_t i = 0; i < 10; ++i)
        {
            value_type val;
            pw::test::permute(val, i);
            v.push_back(val);
        }

        WHEN("resize() to current size")
        {
            size_t const original_size     = v.size();
            size_t const original_capacity = v.capacity();

            v.resize(original_size);
            THEN("nothing changes")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() == original_capacity);
            }
        }

        WHEN("resize() to smaller size")
        {
            constexpr size_t new_size          = 5;
            size_t const     original_capacity = v.capacity();
            Vector           original_values(v);

            v.resize(new_size);
            THEN("size decreases, capacity unchanged")
            {
                REQUIRE(v.size() == new_size);
                REQUIRE(v.capacity() == original_capacity);
            }
            THEN("remaining elements are unchanged")
            {
                for (size_t i = 0; i < new_size; ++i)
                {
                    REQUIRE(v[i] == original_values[i]);
                }
            }
        }
    }

    GIVEN("A vector with capacity > size")
    {
        Vector v;
        v.reserve(20);
        for (size_t i = 0; i < 5; ++i)
        {
            value_type val;
            pw::test::permute(val, i);
            v.push_back(val);
        }

        WHEN("resize() within capacity")
        {
            size_t const     original_capacity = v.capacity();
            constexpr size_t new_size          = 15;
            Vector           original_values(v);

            v.resize(new_size);
            THEN("size increases, capacity unchanged")
            {
                REQUIRE(v.size() == new_size);
                REQUIRE(v.capacity() == original_capacity);
            }
            THEN("original elements preserved")
            {
                for (size_t i = 0; i < original_values.size(); ++i)
                {
                    REQUIRE(v[i] == original_values[i]);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = original_values.size(); i < new_size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }

        WHEN("resize() beyond capacity")
        {
            size_t const original_capacity = v.capacity();
            size_t const new_size          = original_capacity + 10;
            Vector       original_values(v);

            v.resize(new_size);
            THEN("size increases, capacity increases")
            {
                REQUIRE(v.size() == new_size);
                REQUIRE(v.capacity() >= new_size);
                REQUIRE(v.capacity() > original_capacity);
            }
            THEN("original elements preserved")
            {
                for (size_t i = 0; i < original_values.size(); ++i)
                {
                    REQUIRE(v[i] == original_values[i]);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = original_values.size(); i < new_size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
    }
}
SCENARIO("resize() op counts", "[vector][resize][optracker]")
{
    using Vector = pw::vector<pw::test::OpTrackerDefaultCopyConstructible>;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init = pw::test::OpTrackerDefaultCopyConstructible::getCounter();

    GIVEN("An empty vector")
    {
        Vector v;

        WHEN("resize() is called")
        {
            size_t count = 5;
            counter      = pw::test::OpTrackerDefaultCopyConstructible::getCounter();

            v.resize(count);
            counter = pw::test::OpTrackerDefaultCopyConstructible::getCounter() - counter;
            THEN("default constructed count times")
            {
                INFO("counter: " << counter);
                REQUIRE(count == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() == counter.constructorCount());
                REQUIRE(counter.allCount() == count);
            }
        }
    }

    GIVEN("A vector with existing elements")
    {
        Vector v { 1, 2, 3 };

        WHEN("resize() increases size")
        {
            counter = pw::test::OpTrackerDefaultCopyConstructible::getCounter();
            v.resize(v.size() + 2);
            counter = pw::test::OpTrackerDefaultCopyConstructible::getCounter() - counter;
            THEN("new elements are default constructed and original copy constructed")
            {
                // Expect copy construction so exceptions leave unchanged
                INFO("counter: " << counter);
                REQUIRE(2 == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() + counter.getCopyConstructor() ==
                        counter.constructorCount());
            }
        }

        WHEN("resize() decreases size")
        {
            pw::test::OpCounter before = pw::test::OpTrackerDefaultCopyConstructible::getCounter();
            v.resize(1);
            counter = pw::test::OpTrackerDefaultCopyConstructible::getCounter() - before;
            THEN("excess elements are destructed")
            {
                INFO("counter: " << counter);
                REQUIRE(0 == counter.constructorCount());
                REQUIRE(2 == counter.destructorCount());
            }
        }

        WHEN("resize() to 0")
        {
            pw::test::OpCounter before = pw::test::OpTrackerDefaultCopyConstructible::getCounter();
            v.resize(0);
            counter = pw::test::OpTrackerDefaultCopyConstructible::getCounter() - before;
            THEN("all elements are destructed")
            {
                INFO("counter: " << counter);
                REQUIRE(0 == counter.constructorCount());
                REQUIRE(3 == counter.destructorCount());
            }
        }
    }
    counter = pw::test::OpTrackerDefaultCopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

SCENARIO("resize() stress tests", "[vector][resize][stress]")
{
    using Vector = pw::vector<int>;

    GIVEN("A vector")
    {
        Vector v;

        WHEN("multiple resize operations are performed")
        {
            for (size_t i = 0; i < 100; ++i)
            {
                v.resize(i);
                REQUIRE(v.size() == i);
                for (int j : v)
                {
                    REQUIRE(j == 0);
                }
            }

            THEN("all operations succeed")
            {
                REQUIRE(v.size() == 99);
            }

            WHEN("resize down and up repeatedly")
            {
                for (size_t i = 0; i < 10; ++i)
                {
                    v.resize(50);
                    REQUIRE(v.size() == 50);
                    v.resize(100);
                    REQUIRE(v.size() == 100);
                }

                THEN("final state is correct")
                {
                    REQUIRE(v.size() == 100);
                    for (int j : v)
                    {
                        REQUIRE(j == 0);
                    }
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
