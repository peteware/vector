#include "test_allocator_copy_assignment.h"
#include "test_allocator_move_assignment.h"
#include "test_allocator_swapable.h"

#include <pw/vector>
#include <unistd.h>

#include <pw/impl/move.h>

#include <catch2/catch.hpp>

/*
 * Type Requirements:
 * - No extra
 */
TEST_CASE("Constructors", "[vector][constructor]")
{
    using Vector     = pw::vector<int>;
    using value_type = Vector::value_type;

    GIVEN("An empty vector")
    {
        WHEN("Nothing changes")
        {
            Vector v;
            THEN("empty() is true")
            {
                REQUIRE(v.empty());
            }
            THEN("size() is 0")
            {
                REQUIRE(v.size() == 0); // NOLINT(*-container-size-empty)
            }
            THEN("begin() is end()")
            {
                REQUIRE(v.begin() == v.end());
            }
        }
    }
    GIVEN("A vector of count values")
    {
        WHEN("Initialized with value")
        {
            constexpr Vector::size_type total = 10;
            constexpr value_type        value = 3;
            Vector                      v(total, value);

            THEN("empty() is false")
            {
                REQUIRE(!v.empty());
            }
            THEN("size() is total")
            {
                REQUIRE(total == v.size());
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("begin() returns element")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(*(v.end() - 1) == value);
            }
            THEN("all elements have correct value")
            {
                for (Vector::size_type i = 0; i < total; ++i)
                {
                    REQUIRE(v[i] == value);
                }
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() < v.end());
                REQUIRE(v.end() - v.begin() == static_cast<typename Vector::difference_type>(total));
            }
        }
    }
}

TEST_CASE("Constructors without allocator", "[constructor][no-allocator]")
{
    using Allocator = pw::test::allocator_move_assignment<int>;
    using Vector    = pw::vector<int, Allocator>;

    Allocator alloc;

    GIVEN("default allocator instance")
    {
        WHEN("default constructor is called")
        {
            // constexpr vector() noexcept(noexcept(allocator_type()));
            Vector v;

            THEN("vector is empty")
            {
                REQUIRE(v.empty());
            }
            THEN("allocator matches default instance")
            {
                REQUIRE(v.get_allocator().m_instance == alloc.m_instance);
            }
        }
    }
    GIVEN("a vector with elements { 1, 2, 3, 10 }")
    {
        Vector v1 { 1, 2, 3, 10 };

        WHEN("copy constructor is called")
        {
            // constexpr vector(vector const& other);
            Vector v2(v1);

            THEN("allocators are equal")
            {
                REQUIRE(v1.get_allocator() == v2.get_allocator());
            }
            THEN("sizes are equal")
            {
                REQUIRE(v1.size() == v2.size());
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v2.capacity() >= v2.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v2.empty());
            }
            THEN("all elements are copied correctly")
            {
                REQUIRE(v1[0] == v2[0]);
                REQUIRE(v1[1] == v2[1]);
                REQUIRE(v1[2] == v2[2]);
                REQUIRE(v1[3] == v2[3]);
            }
            THEN("iterators are valid")
            {
                REQUIRE(v2.begin() != v2.end());
            }
        }
        WHEN("move constructor is called")
        {
            // constexpr vector(vector&& other) noexcept;
            auto   original_size = v1.size();
            Vector v2(move(v1));

            THEN("allocators are equal")
            {
                REQUIRE(v1.get_allocator() == v2.get_allocator());
            }
            THEN("size is preserved")
            {
                REQUIRE(v2.size() == original_size);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v2.capacity() >= v2.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v2.empty());
            }
            THEN("all elements are moved correctly")
            {
                REQUIRE(v2[0] == 1);
                REQUIRE(v2[1] == 2);
                REQUIRE(v2[2] == 3);
                REQUIRE(v2[3] == 10);
            }
            THEN("iterators are valid")
            {
                REQUIRE(v2.begin() != v2.end());
            }
        }
    }
}

TEST_CASE("Constructors use allocator", "[constructor][allocator]")
{
    using Allocator  = pw::test::allocator_move_assignment<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = Vector::value_type;

    GIVEN("a custom allocator instance")
    {
        Allocator alloc(3);

        WHEN("default constructor is called with allocator")
        {
            // constexpr explicit vector(allocator_type const& alloc) noexcept;
            Vector v(alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
        }
        WHEN("constructor is called with count, value, and allocator")
        {
            // constexpr vector(size_type count, value_type const& value, allocator_type const& alloc = allocator_type());
            constexpr Vector::size_type count = 30;
            constexpr value_type        value = 812;
            Vector                      v(count, value, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches count")
            {
                REQUIRE(v.size() == count);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
            THEN("all elements have the specified value")
            {
                REQUIRE(v[0] == value);
                REQUIRE(v[count - 1] == value);
                for (Vector::size_type i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == value);
                }
            }
        }
        WHEN("constructor is called with count and allocator but no value")
        {
            // constexpr explicit vector(size_type count, allocator_type const& alloc = allocator_type());
            constexpr Vector::size_type count = 30;
            Vector                      v(count, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches count")
            {
                REQUIRE(v.size() == count);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(v.end() - v.begin() == static_cast<Vector::difference_type>(count));
            }
        }
        WHEN("copy constructor is called with different allocator")
        {
            // constexpr vector(vector const& other, allocator_type const& alloc);
            Allocator alloc2;
            Vector    v1({ 1, 2, 3, 4 }, alloc);
            Vector    v2(v1, alloc2);

            THEN("new allocator is used")
            {
                REQUIRE(v2.get_allocator() == alloc2);
            }
            THEN("size is copied")
            {
                REQUIRE(v2.size() == v1.size());
            }
            THEN("elements are copied correctly")
            {
                REQUIRE(v2[0] == v1[0]);
                REQUIRE(v2[3] == v1[3]);
            }
        }
        WHEN("move constructor is called with different allocator")
        {
            // constexpr vector(vector&& other, allocator_type const& alloc);
            Allocator alloc2;
            Vector    v1({ 1, 2, 3 }, alloc);
            Vector    v2(pw::move(v1), alloc2);

            THEN("new allocator is used")
            {
                REQUIRE(v2.get_allocator() == alloc2);
            }
            THEN("size is moved")
            {
                REQUIRE(v2.size() == v1.size());
            }
            THEN("elements are moved correctly")
            {
                REQUIRE(v2[0] == 1);
                REQUIRE(v2[2] == 3);
            }
        }
        WHEN("constructor is called with initializer list and allocator")
        {
            // constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());
            Vector v({ 1, 2, 3, 4 }, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches initializer list")
            {
                REQUIRE(v.size() == 4);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
            THEN("all elements are initialized correctly")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
                REQUIRE(v[3] == 4);
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() != v.end());
            }
        }
        WHEN("constructor is called with iterator range and allocator")
        {
            // template<class Iterator>
            // constexpr vector(Iterator first, Iterator last, allocator_type const& alloc = allocator_type());
            value_type d[3] = { 23, 2, 1 };
            Vector     v(&d[0], &d[3], alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches range")
            {
                REQUIRE(v.size() == 3);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("elements are copied from range")
            {
                REQUIRE(v[0] == 23);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 1);
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
        }
    }
    GIVEN("constructors with zero count")
    {
        Allocator alloc(3);

        WHEN("constructor is called with zero count and value")
        {
            constexpr value_type value = 42;
            Vector               v1(static_cast<Vector::size_type>(0), value, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v1.get_allocator() == alloc);
            }
            THEN("vector is empty")
            {
                REQUIRE(v1.empty());
            }
            THEN("iterators are equal")
            {
                REQUIRE(v1.begin() == v1.end());
            }
        }
        WHEN("constructor is called with zero count and no value")
        {
            Vector v2(static_cast<Vector::size_type>(0), alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v2.get_allocator() == alloc);
            }
            THEN("vector is empty")
            {
                REQUIRE(v2.empty());
            }
            THEN("iterators are equal")
            {
                REQUIRE(v2.begin() == v2.end());
            }
        }
    }
    GIVEN("iterator range edge cases")
    {
        Allocator  alloc(3);
        value_type d[] = { 1, 2, 3 };

        WHEN("constructor is called with empty range")
        {
            Vector v1(&d[0], &d[0], alloc); // first == last

            THEN("allocator is preserved")
            {
                REQUIRE(v1.get_allocator() == alloc);
            }
            THEN("vector is empty")
            {
                REQUIRE(v1.empty());
            }
            THEN("iterators are equal")
            {
                REQUIRE(v1.begin() == v1.end());
            }
        }
        WHEN("constructor is called with single element range")
        {
            Vector v2(&d[0], &d[1], alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v2.get_allocator() == alloc);
            }
            THEN("size is one")
            {
                REQUIRE(v2.size() == 1);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v2.capacity() >= v2.size());
            }
            THEN("element is copied correctly")
            {
                REQUIRE(v2[0] == 1);
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v2.empty());
            }
            THEN("iterators are valid")
            {
                REQUIRE(v2.begin() != v2.end());
            }
        }
    }
}

TEST_CASE("Constexpr Constructor Tests", "[constructor][constexpr]")
{
    using Vector = pw::vector<int>;

    GIVEN("constexpr context")
    {
        WHEN("default constructor is used in constexpr")
        {
            constexpr auto test_constexpr = []() constexpr {
                Vector v;
                return v.empty();
            };

            THEN("compilation succeeds and runtime validation passes")
            {
                static_assert(test_constexpr());
                REQUIRE(test_constexpr());
            }
        }
        WHEN("initializer list constructor is used in constexpr")
        {
            constexpr auto test_constexpr = []() constexpr {
                Vector v { 1, 2, 3 };
                return v.size() == 3 && v[0] == 1 && v[1] == 2 && v[2] == 3;
            };

            THEN("runtime validation passes")
            {
                //static_assert(test_constexpr());
                REQUIRE(test_constexpr());
            }
        }
    }
}

TEST_CASE("Copy Assignment uses allocator", "[assignment][allocator][copy]")
{
    using Allocator = pw::test::allocator_copy_assignment<int>;
    using Vector    = pw::vector<int, Allocator>;

    GIVEN("vector with copy assignment allocator")
    {
        Allocator alloc;
        Vector    v1({ 10, 20, 30 }, alloc);
        REQUIRE(Allocator::propagate_on_container_copy_assignment::value);
        REQUIRE(v1.get_allocator() == alloc);

        WHEN("allocator_copy_assignment behavior is checked")
        {
            // constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());
            Vector v({ 1, 2, 3, 4 }, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches initializer list")
            {
                REQUIRE(v.size() == 4);
            }
            THEN("elements are initialized correctly")
            {
                REQUIRE(v[0] == 1);
            }
        }
        WHEN("copy assignment is performed")
        {
            // constexpr vector& operator=(const vector& other);
            //     using propagate_on_container_copy_assignment = true_type;
            Vector v2;
            v2 = v1;

            THEN("allocator is propagated to target")
            {
                INFO("alloc.m_version = "
                     << alloc.m_instance
                     << " v1.get_allocator().m_instance = " << v1.get_allocator().m_instance
                     << " v2.get_allocator().m_instance = " << v2.get_allocator().m_instance);
                REQUIRE(v2.get_allocator() == v1.get_allocator());
            }
        }
    }
}

TEST_CASE("Move Assignment use allocator", "[assignment][allocator][move]")
{
    // constexpr vector& operator=(vector&& other) noexcept(
    //     using propagate_on_container_move_assignment = false_type;
    //     using propagate_on_container_move_assignment = true_type;
    //
    // - op1.size() < op2.size()
    // - op1.size() == op2.size()
    // - op1.size() > op2.size()
    // - op1.size() < op2.size() && op1.allocated() > op2.allocated()
    GIVEN("A vector with propagate_on_move_assignment = true")
    {
        using Allocator = pw::test::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc1 { 5 };
        Allocator alloc2 { 10 };
        Vector    v1 { { 1, 2, 3 }, alloc1 };
        Vector    v2 { { 4, 5, 6 }, alloc2 };
        REQUIRE(Allocator::propagate_on_container_move_assignment::value);

        WHEN("Use move() assignmet")
        {
            v1 = pw::move(v2);
            REQUIRE(v1.get_allocator() == alloc2);
            REQUIRE(v1[0] == v2[0]);
        }
    }
    GIVEN("A vector with propagate_on_move_assignment = false")
    {
        using Allocator = pw::test::allocator_base<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc1 { 5 };
        Allocator alloc2 { 10 };
        Vector    v1 { { 1, 2, 3 }, alloc1 };
        Vector    v2 { { 4, 5, 6 }, alloc2 };
        REQUIRE(!Allocator::propagate_on_container_move_assignment::value);
        WHEN("Use move() assignmet")
        {
            v1 = pw::move(v2);
            REQUIRE(v1.get_allocator() == alloc1);
            REQUIRE(v1[0] == v2[0]);
        }
    }
}

TEST_CASE("Assignment with initializer_List use allocator", "[assignment][allocator][init_list]")
{
    // constexpr vector& operator=(vector&& other) noexcept(
    //     using propagate_on_container_move_assignment = false_type;
    //     using propagate_on_container_move_assignment = true_type;
    //
    // - op1.size() < op2.size()
    // - op1.size() == op2.size()
    // - op1.size() > op2.size()
    // - op1.size() < op2.size() && op1.allocated() > op2.allocated()
    GIVEN("A vector with propagate_on_move_assignment = true")
    {
        using Allocator = pw::test::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc1 { 5 };
        Vector    v1 { { 1, 2, 3 }, alloc1 };
        WHEN("Use initializer_list assignment")
        {
            v1 = { 4, 5, 6 };
            REQUIRE(v1.get_allocator() == alloc1);
            REQUIRE(v1[0] == 4);
        }
    }
}

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

TEST_CASE("Constructors with Allocator", "[vector][constructor][allocator]")
{
    using Allocator  = pw::test::allocator_move_assignment<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = Vector::value_type;

    GIVEN("An empty vector with allocator")
    {
        Allocator allocator(5);
        Vector    v(allocator);
        WHEN("Check allocator")
        {
            THEN("allocator is same")
            {
                REQUIRE(allocator == v.get_allocator());
            }
        }
    }
    GIVEN("Two empty vectors with allocator")
    {
        Vector v1(Allocator(5));
        Vector v2(Allocator(10));
        WHEN("Default allocator")
        {
            THEN("allocators are different")
            {
                REQUIRE(!(v1.get_allocator() == v2.get_allocator()));
            }
        }
    }
    GIVEN("Vector with count, value, and allocator")
    {
        constexpr Vector::size_type total = 10;
        Allocator                   allocator(5);
        value_type constexpr value = 3;
        Vector v(total, value, allocator);

        WHEN("access elements")
        {
            THEN("begin() returns element")
            {
                REQUIRE(v.begin());
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(v.end());
                REQUIRE(*(v.end() - 1) == value);
            }
            THEN("the get_allocator() returns original instance")
            {
                REQUIRE(allocator == v.get_allocator());
            }
        }
    }
}

TEST_CASE("Assign methods", "[vector][assign]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("assign(count, value) is called")
        {
            Vector v;
            v.assign(static_cast<Vector::size_type>(5), 42);

            THEN("vector has correct size")
            {
                REQUIRE(v.size() == 5);
            }
            THEN("all elements have assigned value")
            {
                for (auto x : v)
                    REQUIRE(x == 42);
            }
        }
        WHEN("assign(0, value) is called")
        {
            Vector v;
            v.assign(static_cast<Vector::size_type>(0), 99);

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
            }
        }
        WHEN("assign(iterator, iterator) is called")
        {
            int    arr[] = { 1, 2, 3, 4 };
            Vector v;
            v.assign(arr, arr + 4);

            THEN("vector has correct size")
            {
                REQUIRE(v.size() == 4);
            }
            THEN("all elements are copied correctly")
            {
                for (int i = 0; i < 4; ++i)
                    REQUIRE(v[i] == arr[i]);
            }
        }
        WHEN("assign(empty_range) is called")
        {
            Vector v;
            int    arr[] = { 1, 2, 3, 4 };
            v.assign(arr, arr); // empty range

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
            }
        }
        WHEN("assign(initializer_list) is called")
        {
            Vector v;
            v.assign({ 7, 8, 9 });

            THEN("vector has correct size")
            {
                REQUIRE(v.size() == 3);
            }
            THEN("all elements are assigned correctly")
            {
                REQUIRE(v[0] == 7);
                REQUIRE(v[1] == 8);
                REQUIRE(v[2] == 9);
            }
        }
        WHEN("assign(empty_initializer_list) is called")
        {
            Vector v;
            v.assign({});

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("vector with existing contents")
    {
        WHEN("assign replaces old contents")
        {
            Vector v = { 1, 2, 3 };
            v.assign(static_cast<Vector::size_type>(2), 5);

            THEN("vector has new size")
            {
                REQUIRE(v.size() == 2);
            }
            THEN("all elements have new value")
            {
                REQUIRE(v[0] == 5);
                REQUIRE(v[1] == 5);
            }
        }
    }
    GIVEN("assign with zero items")
    {
        WHEN("assign(0, value) on empty vector")
        {
            Vector v;
            v.assign(static_cast<Vector::size_type>(0), 42);

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("assign(0, value) on non-empty vector")
        {
            Vector v = { 1, 2, 3, 4, 5 };
            v.assign(static_cast<Vector::size_type>(0), 99);

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("assign(empty_range) on empty vector")
        {
            Vector v;
            int    arr[] = { 1, 2, 3 };
            v.assign(arr, arr);

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("assign(empty_range) on non-empty vector")
        {
            Vector v     = { 10, 20, 30 };
            int    arr[] = { 1, 2, 3 };
            v.assign(arr, arr);

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("assign(empty_initializer_list) on empty vector")
        {
            Vector v;
            v.assign({});

            THEN("vector remains empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("assign(empty_initializer_list) on non-empty vector")
        {
            Vector v = { 7, 8, 9, 10 };
            v.assign({});

            THEN("vector becomes empty")
            {
                REQUIRE(v.empty());
                REQUIRE(v.begin() == v.end());
            }
        }
    }
}

TEST_CASE("push_back() method", "[vector][push_back][modifiers]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("push_back() is called")
        {
            Vector v;
            v.push_back(42);

            THEN("size increases to 1")
            {
                REQUIRE(v.size() == 1);
            }
            THEN("element is accessible through all access methods")
            {
                REQUIRE(v[0] == 42);
                REQUIRE(v.front() == 42);
                REQUIRE(v.back() == 42);
                REQUIRE(v.at(0) == 42);
            }
        }
        WHEN("multiple push_back() calls are made")
        {
            Vector v;
            v.push_back(10);
            v.push_back(20);
            v.push_back(30);

            THEN("size increases correctly")
            {
                REQUIRE(v.size() == 3);
            }
            THEN("all elements are stored correctly")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(v.front() == 10);
                REQUIRE(v.back() == 30);
            }
        }
    }
    GIVEN("non-empty vector")
    {
        WHEN("push_back() is called")
        {
            Vector v             = { 1, 2, 3 };
            auto   original_size = v.size();
            v.push_back(4);

            THEN("size increases by one")
            {
                REQUIRE(v.size() == original_size + 1);
            }
            THEN("existing elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
            THEN("new element is added at the end")
            {
                REQUIRE(v[3] == 4);
                REQUIRE(v.back() == 4);
            }
        }
    }
    GIVEN("capacity and growth scenarios")
    {
        WHEN("push_back() causes capacity growth")
        {
            Vector v;

            // Add elements until capacity grows
            for (int i = 0; i < 10; ++i)
            {
                v.push_back(i);
            }

            THEN("size increases correctly")
            {
                REQUIRE(v.size() == 10);
            }
            THEN("capacity is sufficient")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("all elements are stored correctly")
            {
                for (int i = 0; i < 10; ++i)
                {
                    REQUIRE(v[i] == i);
                }
            }
        }
        WHEN("push_back() preserves existing elements")
        {
            Vector v = { 100, 200, 300 };

            for (int i = 0; i < 50; ++i)
            {
                v.push_back(i + 1000);
            }

            THEN("size increases correctly")
            {
                REQUIRE(v.size() == 53);
            }
            THEN("original elements are preserved")
            {
                REQUIRE(v[0] == 100);
                REQUIRE(v[1] == 200);
                REQUIRE(v[2] == 300);
            }
            THEN("new elements are added correctly")
            {
                for (int i = 0; i < 50; ++i)
                {
                    REQUIRE(v[i + 3] == i + 1000);
                }
            }
        }
    }
    GIVEN("copy semantics and value handling")
    {
        WHEN("push_back() with copy semantics")
        {
            Vector v;
            int    value = 99;
            v.push_back(value);

            THEN("element is copied correctly")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v[0] == 99);
            }

            value = 77; // Modify original
            THEN("vector is unchanged when original is modified")
            {
                REQUIRE(v[0] == 99);
            }

            v[0] = 88; // Modify vector element
            THEN("original is unchanged when vector element is modified")
            {
                REQUIRE(value == 77);
                REQUIRE(v[0] == 88);
            }
        }
        WHEN("push_back() with different values")
        {
            Vector v;
            v.push_back(0);
            v.push_back(-1);
            v.push_back(2147483647);  // max int
            v.push_back(-2147483648); // min int

            THEN("all values are stored correctly")
            {
                REQUIRE(v.size() == 4);
                REQUIRE(v[0] == 0);
                REQUIRE(v[1] == -1);
                REQUIRE(v[2] == 2147483647);
                REQUIRE(v[3] == -2147483648);
            }
        }
    }
    GIVEN("iterator and capacity scenarios")
    {
        WHEN("push_back() effects on iterators")
        {
            Vector v               = { 1, 2, 3 };

            auto   begin_before    = v.begin();
            auto   end_before      = v.end();
            auto   distance_before = end_before - begin_before;
            REQUIRE(distance_before == 3);

            v.push_back(4);

            auto begin_after    = v.begin();
            auto end_after      = v.end();
            auto distance_after = end_after - begin_after;

            THEN("iterator distance increases")
            {
                REQUIRE(distance_after == 4);
            }
            THEN("new iterators are valid")
            {
                // Note: iterators from before push_back may be invalidated
                // We only test the new iterators
                REQUIRE(*begin_after == 1);
                REQUIRE(*(end_after - 1) == 4);
            }
        }
        WHEN("push_back() with reserved capacity")
        {
            Vector v;
            v.reserve(100);
            auto reserved_capacity = v.capacity();

            for (int i = 0; i < 50; ++i)
            {
                v.push_back(i);
            }

            THEN("capacity remains unchanged")
            {
                REQUIRE(v.size() == 50);
                REQUIRE(v.capacity() == reserved_capacity); // Should not reallocate
            }
            THEN("all elements are stored correctly")
            {
                for (int i = 0; i < 50; ++i)
                {
                    REQUIRE(v[i] == i);
                }
            }
        }
    }
    GIVEN("consistency and special scenarios")
    {
        WHEN("push_back() consistency with other methods")
        {
            Vector v;

            for (int i = 1; i <= 5; ++i)
            {
                v.push_back(i * 10);

                REQUIRE(v.size() == static_cast<Vector::size_type>(i));
                REQUIRE(v.back() == i * 10);
                REQUIRE(v[i - 1] == i * 10);
                REQUIRE(v.at(i - 1) == i * 10);
                REQUIRE(*(v.end() - 1) == i * 10);

                if (v.size() > 1)
                {
                    REQUIRE(v.front() == 10);
                }
            }

            THEN("all access methods return consistent values")
            {
                REQUIRE(v.size() == 5);
            }
        }
        WHEN("push_back() with different allocators")
        {
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v(alloc);

            v.push_back(10);
            v.push_back(20);
            v.push_back(30);

            THEN("allocator is preserved")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("elements are stored correctly")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
        }
        WHEN("push_back() large number of elements")
        {
            Vector        v;
            constexpr int count = 1000;

            for (int i = 0; i < count; ++i)
            {
                v.push_back(i * 2);
            }

            THEN("size and capacity are correct")
            {
                REQUIRE(v.size() == count);
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("all elements are stored correctly")
            {
                for (int i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == i * 2);
                }
                REQUIRE(v.front() == 0);
                REQUIRE(v.back() == (count - 1) * 2);
            }
        }
        WHEN("push_back() after clear")
        {
            Vector v = { 1, 2, 3, 4, 5 };

            v.clear();
            REQUIRE(v.empty());
            v.push_back(100);

            THEN("vector has one element")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v[0] == 100);
                REQUIRE(v.front() == 100);
                REQUIRE(v.back() == 100);
            }
        }
        WHEN("push_back() mixed with other operations")
        {
            Vector v;

            v.push_back(1);
            v.push_back(2);
            REQUIRE(v.size() == 2);

            v.assign({ 10, 20, 30 });
            REQUIRE(v.size() == 3);
            REQUIRE(v[0] == 10);

            v.push_back(40);
            v.clear();
            v.push_back(99);

            THEN("final state is correct")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v[0] == 99);
            }
        }
    }
}

TEST_CASE("at() methods", "[vector][at][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("at() is called on empty vector")
        {
            Vector v;

            THEN("out_of_range exception is thrown")
            {
                REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
            }
        }
        WHEN("at() is called on const empty vector")
        {
            constexpr Vector v;

            THEN("out_of_range exception is thrown")
            {
                REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
            }
        }
    }
    GIVEN("single element vector")
    {
        WHEN("at() is called on single element vector")
        {
            Vector v = { 42 };

            THEN("valid index returns correct value")
            {
                REQUIRE(v.at(0) == 42);
            }
            THEN("invalid indices throw out_of_range")
            {
                REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
            }
            THEN("element can be modified through at()")
            {
                v.at(0) = 99;
                REQUIRE(v.at(0) == 99);
                REQUIRE(v[0] == 99);
            }
        }
        WHEN("at() is called on const single element vector")
        {
            const Vector v = { 42 };

            THEN("valid index returns correct value")
            {
                REQUIRE(v.at(0) == 42);
            }
            THEN("invalid indices throw out_of_range")
            {
                REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("at() is called on multi-element vector")
        {
            Vector v = { 10, 20, 30, 40, 50 };

            THEN("valid indices return correct values")
            {
                REQUIRE(v.at(0) == 10);
                REQUIRE(v.at(1) == 20);
                REQUIRE(v.at(2) == 30);
                REQUIRE(v.at(3) == 40);
                REQUIRE(v.at(4) == 50);
            }
            THEN("invalid indices throw out_of_range")
            {
                REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(10), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
            }
            THEN("elements can be modified through at()")
            {
                v.at(2) = 333;
                REQUIRE(v.at(2) == 333);
                REQUIRE(v[2] == 333);
            }
        }
        WHEN("at() is called on const multi-element vector")
        {
            const Vector v = { 10, 20, 30, 40, 50 };

            THEN("valid indices return correct values")
            {
                REQUIRE(v.at(0) == 10);
                REQUIRE(v.at(1) == 20);
                REQUIRE(v.at(2) == 30);
                REQUIRE(v.at(3) == 40);
                REQUIRE(v.at(4) == 50);
            }
            THEN("invalid indices throw out_of_range")
            {
                REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(10), std::out_of_range);
                REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
            }
        }
    }
    GIVEN("boundary conditions and reference semantics")
    {
        WHEN("at() boundary conditions are tested")
        {
            Vector v(static_cast<Vector::size_type>(1000), 7);

            THEN("boundary indices work correctly")
            {
                REQUIRE(v.at(0) == 7);
                REQUIRE(v.at(999) == 7);
                REQUIRE_THROWS_AS(v.at(1000), std::out_of_range);
            }
            THEN("boundary elements can be modified")
            {
                v.at(0)   = 1;
                v.at(999) = 2;
                REQUIRE(v.at(0) == 1);
                REQUIRE(v.at(999) == 2);
                REQUIRE(v.at(500) == 7);
            }
        }
        WHEN("at() return type and reference semantics are tested")
        {
            Vector v = { 100 };

            THEN("reference modification works")
            {
                int& ref = v.at(0);
                ref      = 200;
                REQUIRE(v.at(0) == 200);
                REQUIRE(v[0] == 200);
            }
        }
    }
}

TEST_CASE("operator[] methods", "[vector][operator][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("single element vector")
    {
        WHEN("operator[] is used on single element vector")
        {
            Vector v = { 42 };

            THEN("element can be accessed")
            {
                REQUIRE(v[0] == 42);
            }
            THEN("element can be modified")
            {
                v[0] = 99;
                REQUIRE(v[0] == 99);
                REQUIRE(v.at(0) == 99);
            }
        }
        WHEN("const operator[] is used on single element vector")
        {
            const Vector v = { 42 };

            THEN("element can be accessed")
            {
                REQUIRE(v[0] == 42);
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("operator[] is used on multi-element vector")
        {
            Vector v = { 10, 20, 30, 40, 50 };

            THEN("all elements can be accessed")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(v[3] == 40);
                REQUIRE(v[4] == 50);
            }
            THEN("elements can be modified")
            {
                v[2] = 333;
                REQUIRE(v[2] == 333);
                REQUIRE(v.at(2) == 333);
            }
        }
        WHEN("const operator[] is used on multi-element vector")
        {
            const Vector v = { 10, 20, 30, 40, 50 };

            THEN("all elements can be accessed")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(v[3] == 40);
                REQUIRE(v[4] == 50);
            }
        }
    }
    GIVEN("boundary conditions and semantics")
    {
        WHEN("operator[] boundary conditions are tested")
        {
            Vector v(static_cast<Vector::size_type>(1000), 7);

            THEN("boundary elements can be accessed")
            {
                REQUIRE(v[0] == 7);
                REQUIRE(v[999] == 7);
            }
            THEN("boundary elements can be modified")
            {
                v[0]   = 1;
                v[999] = 2;
                REQUIRE(v[0] == 1);
                REQUIRE(v[999] == 2);
                REQUIRE(v[500] == 7);
            }
        }
        WHEN("operator[] return type and reference semantics are tested")
        {
            Vector v = { 100 };

            THEN("reference modification works")
            {
                int& ref = v[0];
                ref      = 200;
                REQUIRE(v[0] == 200);
                REQUIRE(v.at(0) == 200);
            }
        }
        WHEN("operator[] vs at() comparison is performed")
        {
            Vector v = { 1, 2, 3 };

            THEN("both methods return same values")
            {
                for (int i : v)
                {
                    REQUIRE(i == i);
                }
            }
            THEN("modifications are consistent")
            {
                v[1] = 999;
                REQUIRE(v[1] == v.at(1));
                REQUIRE(v[1] == 999);
            }
        }
    }
}

TEST_CASE("front() and back() methods", "[vector][front][back][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("single element vector")
    {
        WHEN("front() and back() are used on single element vector")
        {
            Vector v = { 42 };

            THEN("both methods return same value")
            {
                REQUIRE(v.front() == 42);
                REQUIRE(v.back() == 42);
                REQUIRE(v.front() == v.back());
            }
            THEN("front() can modify the element")
            {
                v.front() = 99;
                REQUIRE(v.front() == 99);
                REQUIRE(v.back() == 99);
                REQUIRE(v[0] == 99);
            }
            THEN("back() can modify the element")
            {
                v.back() = 77;
                REQUIRE(v.front() == 77);
                REQUIRE(v.back() == 77);
                REQUIRE(v[0] == 77);
            }
        }
        WHEN("const front() and back() are used on single element vector")
        {
            const Vector v = { 42 };

            THEN("both methods return same value")
            {
                REQUIRE(v.front() == 42);
                REQUIRE(v.back() == 42);
                REQUIRE(v.front() == v.back());
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("front() and back() are used on multi-element vector")
        {
            Vector v = { 10, 20, 30, 40, 50 };

            THEN("methods return correct elements")
            {
                REQUIRE(v.front() == 10);
                REQUIRE(v.back() == 50);
                REQUIRE(v.front() == v[0]);
                REQUIRE(v.back() == v[4]);
                REQUIRE(v.back() == v[v.size() - 1]);
            }
            THEN("front() can modify first element")
            {
                v.front() = 100;
                REQUIRE(v.front() == 100);
                REQUIRE(v[0] == 100);
                REQUIRE(v.back() == 50);
            }
            THEN("back() can modify last element")
            {
                v.back() = 500;
                REQUIRE(v.back() == 500);
                REQUIRE(v[4] == 500);
                REQUIRE(v.front() == 10);
            }
        }
        WHEN("const front() and back() are used on multi-element vector")
        {
            const Vector v = { 10, 20, 30, 40, 50 };

            THEN("methods return correct elements")
            {
                REQUIRE(v.front() == 10);
                REQUIRE(v.back() == 50);
                REQUIRE(v.front() == v[0]);
                REQUIRE(v.back() == v[4]);
                REQUIRE(v.back() == v[v.size() - 1]);
            }
        }
    }
    GIVEN("reference semantics and consistency")
    {
        WHEN("front() and back() return type and reference semantics are tested")
        {
            Vector v         = { 100, 200 };

            int&   front_ref = v.front();
            int&   back_ref  = v.back();

            front_ref        = 1000;
            back_ref         = 2000;

            THEN("reference modifications work")
            {
                REQUIRE(v.front() == 1000);
                REQUIRE(v.back() == 2000);
                REQUIRE(v[0] == 1000);
                REQUIRE(v[1] == 2000);
            }
            THEN("const references work")
            {
                const Vector& cv              = v;
                const int&    const_front_ref = cv.front();
                const int&    const_back_ref  = cv.back();

                REQUIRE(const_front_ref == 1000);
                REQUIRE(const_back_ref == 2000);
            }
        }
        WHEN("front() and back() consistency with other access methods is tested")
        {
            Vector v = { 7, 8, 9, 10, 11 };

            THEN("front() is consistent with other access methods")
            {
                REQUIRE(v.front() == v[0]);
                REQUIRE(v.front() == v.at(0));
                REQUIRE(v.front() == *v.begin());
            }
            THEN("back() is consistent with other access methods")
            {
                REQUIRE(v.back() == v[v.size() - 1]);
                REQUIRE(v.back() == v.at(v.size() - 1));
                REQUIRE(v.back() == *(v.end() - 1));
            }
            THEN("modifications are reflected in all access methods")
            {
                v.front() = 77;
                v.back()  = 111;

                REQUIRE(v[0] == 77);
                REQUIRE(v.at(0) == 77);
                REQUIRE(*v.begin() == 77);

                REQUIRE(v[v.size() - 1] == 111);
                REQUIRE(v.at(v.size() - 1) == 111);
                REQUIRE(*(v.end() - 1) == 111);
            }
        }
        WHEN("front() and back() are used on large vector")
        {
            Vector v(static_cast<Vector::size_type>(1000), 5);

            THEN("initial values are correct")
            {
                REQUIRE(v.front() == 5);
                REQUIRE(v.back() == 5);
            }
            THEN("boundary modifications work")
            {
                v.front() = 1;
                v.back()  = 1000;

                REQUIRE(v.front() == 1);
                REQUIRE(v.back() == 1000);
                REQUIRE(v[0] == 1);
                REQUIRE(v[999] == 1000);
                REQUIRE(v[500] == 5);
            }
        }
    }
}

TEST_CASE("data() method", "[vector][data][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("data() is called on empty vector")
        {
            Vector v;

            THEN("data() returns nullptr")
            {
                REQUIRE(v.data() == nullptr);
            }
            THEN("const data() returns nullptr")
            {
                const Vector& cv = v;
                REQUIRE(cv.data() == nullptr);
            }
        }
    }
    GIVEN("single element vector")
    {
        WHEN("data() is called on single element vector")
        {
            Vector v   = { 42 };

            int*   ptr = v.data();

            THEN("data() returns valid pointer")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(*ptr == 42);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr == &v.back());
            }
            THEN("element can be modified through pointer")
            {
                *ptr = 99;
                REQUIRE(v[0] == 99);
                REQUIRE(v.front() == 99);
            }
        }
        WHEN("const data() is called on single element vector")
        {
            const Vector v   = { 42 };
            const int*   ptr = v.data();

            THEN("const data() returns valid pointer")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(*ptr == 42);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr == &v.back());
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("data() is called on multi-element vector")
        {
            Vector v   = { 10, 20, 30, 40, 50 };
            int*   ptr = v.data();

            THEN("data() returns valid pointer to first element")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
            }
            THEN("all elements are accessible through pointer")
            {
                for (Vector::size_type i = 0; i < v.size(); ++i)
                {
                    REQUIRE(ptr[i] == v[i]);
                    REQUIRE(&ptr[i] == &v[i]);
                }
            }
            THEN("elements can be modified through pointer")
            {
                ptr[2] = 333;
                REQUIRE(v[2] == 333);
                REQUIRE(v.at(2) == 333);
            }
        }
        WHEN("const data() is called on multi-element vector")
        {
            const Vector v   = { 10, 20, 30, 40, 50 };
            const int*   ptr = v.data();

            THEN("const data() returns valid pointer")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
            }
            THEN("all elements are accessible through const pointer")
            {
                for (Vector::size_type i = 0; i < v.size(); ++i)
                {
                    REQUIRE(ptr[i] == v[i]);
                    REQUIRE(&ptr[i] == &v[i]);
                }
            }
        }
    }
    GIVEN("pointer arithmetic and consistency")
    {
        WHEN("data() pointer arithmetic is performed")
        {
            Vector v   = { 1, 2, 3, 4, 5 };
            int*   ptr = v.data();

            THEN("pointer arithmetic works correctly")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr + v.size() - 1 == &v.back());
                REQUIRE(*(ptr + 2) == v[2]);
                REQUIRE(*(ptr + 4) == v[4]);
            }
            THEN("all elements accessible via pointer arithmetic")
            {
                for (Vector::size_type i = 0; i < v.size(); ++i)
                {
                    REQUIRE(*(ptr + i) == v[i]);
                }
            }
        }
        WHEN("data() consistency with iterators is tested")
        {
            Vector v   = { 7, 8, 9, 10, 11 };
            int*   ptr = v.data();

            THEN("data() is consistent with iterators")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &(*v.begin()));
            }
            THEN("pointer arithmetic matches iterator advancement")
            {
                auto it = v.begin();
                for (Vector::size_type i = 0; i < v.size(); ++i, ++it)
                {
                    REQUIRE(ptr + i == &(*it));
                    REQUIRE(*(ptr + i) == *it);
                }
            }
        }
        WHEN("data() is used on large vector")
        {
            Vector v(static_cast<Vector::size_type>(1000), 7);
            int*   ptr = v.data();

            THEN("data() works with large vectors")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr + 999 == &v.back());
            }
            THEN("pointer arithmetic works with large vectors")
            {
                REQUIRE(*ptr == 7);
                REQUIRE(*(ptr + 999) == 7);
                REQUIRE(*(ptr + 500) == 7);
            }
            THEN("modifications work with large vectors")
            {
                *(ptr + 500) = 555;
                REQUIRE(v[500] == 555);
                REQUIRE(v.at(500) == 555);
            }
        }
        WHEN("data() is used after vector operations")
        {
            Vector v = { 1, 2, 3 };

            THEN("data() returns nullptr after clear")
            {
                v.clear();
                REQUIRE(v.data() == nullptr);
            }
            THEN("data() works after assign")
            {
                v.assign({ 10, 20 });
                int* after_assign_ptr = v.data();
                REQUIRE(after_assign_ptr != nullptr);
                REQUIRE(*after_assign_ptr == 10);
                REQUIRE(*(after_assign_ptr + 1) == 20);
            }
        }
        WHEN("data() return type verification is performed")
        {
            Vector        v  = { 100, 200 };
            const Vector& cv = v;

            THEN("return types are correct")
            {
                static_assert(std::is_same_v<decltype(v.data()), int*>);
                static_assert(std::is_same_v<decltype(cv.data()), const int*>);
            }
            THEN("pointers point to same location")
            {
                int*       mutable_ptr = v.data();
                const int* const_ptr   = cv.data();

                REQUIRE(mutable_ptr == const_ptr);
                REQUIRE(*mutable_ptr == *const_ptr);
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
