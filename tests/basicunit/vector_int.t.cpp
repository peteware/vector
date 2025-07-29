#include "basicunit_allocator_copy_assignment.h"
#include "basicunit_allocator_move_assignment.h"
#include "basicunit_allocator_swapable.h"

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
    using value_type = typename Vector::value_type;

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
                REQUIRE(!v.size());
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
            typename Vector::size_type const total = 10;
            value_type const                 value = 3;
            Vector                           v(total, value);

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
                for (typename Vector::size_type i = 0; i < total; ++i)
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
    using Allocator = basicunit::allocator_move_assignment<int>;
    using Vector    = pw::vector<int, Allocator>;

    Allocator alloc;

    SECTION("Default constructor")
    {
        // constexpr vector() noexcept(noexcept(allocator_type()));
        Vector v;

        REQUIRE(v.empty());
        REQUIRE(v.get_allocator().m_instance == alloc.m_instance);
    }
    SECTION("Copy constructor")
    {
        // constexpr vector(vector const& other);
        Vector v1 { 1, 2, 3, 10 };
        Vector v2(v1);

        REQUIRE(v1.get_allocator() == v2.get_allocator());
        REQUIRE(v1.size() == v2.size());
        REQUIRE(v2.capacity() >= v2.size());
        REQUIRE(!v2.empty());
        REQUIRE(v1[0] == v2[0]);
        REQUIRE(v1[1] == v2[1]);
        REQUIRE(v1[2] == v2[2]);
        REQUIRE(v1[3] == v2[3]);
        REQUIRE(v2.begin() != v2.end());
    }
    SECTION("Move constructor")
    {
        // constexpr vector(vector&& other) noexcept;
        Vector v1 { 1, 2, 3, 10 };
        auto   original_size = v1.size();
        Vector v2(move(v1));

        REQUIRE(v1.get_allocator() == v2.get_allocator());
        REQUIRE(v2.size() == original_size);
        REQUIRE(v2.capacity() >= v2.size());
        REQUIRE(!v2.empty());
        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 2);
        REQUIRE(v2[2] == 3);
        REQUIRE(v2[3] == 10);
        REQUIRE(v2.begin() != v2.end());
    }
}

TEST_CASE("Constructors use allocator", "[constructor][allocator]")
{
    using Allocator  = basicunit::allocator_move_assignment<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = typename Vector::value_type;

    Allocator alloc(3);
    SECTION("Default constructor with allocator")
    {
        // constexpr explicit vector(allocator_type const& alloc) noexcept;
        Vector v(alloc);

        REQUIRE(v.get_allocator() == alloc);
    }
    SECTION("Constructor with count, value, and allocator")
    {
        // constexpr vector(size_type count, value_type const& value, allocator_type const& alloc = allocator_type());
        Vector::size_type const count = 30;
        value_type const        value = 812;
        Vector                  v(count, value, alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(!v.empty());
        REQUIRE(v[0] == value);
        REQUIRE(v[count - 1] == value);
        for (Vector::size_type i = 0; i < count; ++i)
        {
            REQUIRE(v[i] == value);
        }
    }
    SECTION("Constructor with count, no value, and allocator")
    {
        // constexpr explicit vector(size_type count, allocator_type const& alloc = allocator_type());
        Vector::size_type const count = 30;
        Vector                  v(count, alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(!v.empty());
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.end() - v.begin() == static_cast<Vector::difference_type>(count));
    }
    SECTION("Copy constructor with allocator")
    {
        // constexpr vector(vector const& other, allocator_type const& alloc);
        Allocator alloc2;
        Vector    v1({ 1, 2, 3, 4 }, alloc);
        Vector    v2(v1, alloc2);

        REQUIRE(v2.get_allocator() == alloc2);
        REQUIRE(v2.size() == v1.size());
        REQUIRE(v2[0] == v1[0]); // do without operator==()
        REQUIRE(v2[3] == v1[3]);
    }
    SECTION("Move constructor with allocator")
    {
        // constexpr vector(vector&& other, allocator_type const& alloc);
        Allocator alloc2;
        Vector    v1({ 1, 2, 3 }, alloc);
        Vector    v2(pw::move(v1), alloc2);

        REQUIRE(v2.get_allocator() == alloc2);
        REQUIRE(v2.size() == v1.size());
        REQUIRE(v2[0] == 1);
        REQUIRE(v2[2] == 3);
    }
    SECTION("Construct from initializer list with allocator")
    {
        // constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());
        Vector v({ 1, 2, 3, 4 }, alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == 4);
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(!v.empty());
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 4);
        REQUIRE(v.begin() != v.end());
    }
    SECTION("Construct from iterator with allocator")
    {
        // template<class Iterator>
        // constexpr vector(Iterator first, Iterator last, allocator_type const& alloc = allocator_type());
        value_type d[3] = { 23, 2, 1 };
        Vector     v(&d[0], &d[3], alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(v[0] == 23);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 1);
        REQUIRE(!v.empty());
    }
    SECTION("Zero count constructors")
    {
        // Test zero count with value
        value_type const value = 42;
        Vector           v1(static_cast<Vector::size_type>(0), value, alloc);

        REQUIRE(v1.get_allocator() == alloc);
        REQUIRE(v1.empty());
        REQUIRE(v1.size() == 0);
        REQUIRE(v1.begin() == v1.end());

        // Test zero count without value
        Vector v2(static_cast<Vector::size_type>(0), alloc);

        REQUIRE(v2.get_allocator() == alloc);
        REQUIRE(v2.empty());
        REQUIRE(v2.size() == 0);
        REQUIRE(v2.begin() == v2.end());
    }
    SECTION("Iterator range edge cases")
    {
        // Empty range
        value_type d[] = { 1, 2, 3 };
        Vector     v1(&d[0], &d[0], alloc); // first == last

        REQUIRE(v1.get_allocator() == alloc);
        REQUIRE(v1.empty());
        REQUIRE(v1.size() == 0);
        REQUIRE(v1.begin() == v1.end());

        // Single element range
        Vector v2(&d[0], &d[1], alloc);

        REQUIRE(v2.get_allocator() == alloc);
        REQUIRE(v2.size() == 1);
        REQUIRE(v2.capacity() >= v2.size());
        REQUIRE(v2[0] == 1);
        REQUIRE(!v2.empty());
        REQUIRE(v2.begin() != v2.end());
    }
}

TEST_CASE("Constexpr Constructor Tests", "[constructor][constexpr]")
{
    using Vector = pw::vector<int>;

    SECTION("Constexpr default constructor")
    {
        constexpr auto test_constexpr = []() constexpr {
            Vector v;
            return v.empty() && v.size() == 0;
        };

        static_assert(test_constexpr());
        REQUIRE(test_constexpr());
    }
    SECTION("Constexpr initializer list constructor")
    {
        constexpr auto test_constexpr = []() constexpr {
            Vector v { 1, 2, 3 };
            return v.size() == 3 && v[0] == 1 && v[1] == 2 && v[2] == 3;
        };

        //static_assert(test_constexpr());
        REQUIRE(test_constexpr());
    }
}

TEST_CASE("Copy Assignment uses allocator", "[assignment][allocator][copy]")
{
    using Allocator = basicunit::allocator_copy_assignment<int>;
    using Vector    = pw::vector<int, Allocator>;

    Allocator alloc;
    Vector    v1({ 10, 20, 30 }, alloc);
    REQUIRE(Allocator::propagate_on_container_copy_assignment::value);
    REQUIRE(v1.get_allocator() == alloc);
    SECTION("Check that allocator_copy_assignment behaves")
    {
        // constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());
        Vector v({ 1, 2, 3, 4 }, alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 1);
    }
    SECTION("Assignment with allocator that does copy on assignment creates "
            "new allocator")
    {
        // constexpr vector& operator=(const vector& other);
        //     using propagate_on_container_copy_assignment = true_type;
        Vector v2;

        v2 = v1;
        INFO("alloc.m_version = " << alloc.m_instance
                                  << " v1.get_allocator().m_instance = " << v1.get_allocator().m_instance
                                  << " v2.get_allocator().m_instance = " << v2.get_allocator().m_instance);
        REQUIRE(v2.get_allocator() == v1.get_allocator());
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
        using Allocator = basicunit::allocator_move_assignment<int>;
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
        using Allocator = basicunit::allocator_base<int>;
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
        using Allocator = basicunit::allocator_move_assignment<int>;
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
        using Allocator = basicunit::allocator_swapable<int>;
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
        using Allocator = basicunit::allocator_base<int>;
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
    using Allocator  = basicunit::allocator_move_assignment<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = typename Vector::value_type;

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
        typename Vector::size_type const total = 10;
        Allocator                        allocator(5);
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
    using Vector     = pw::vector<int>;
    using value_type = typename Vector::value_type;

    SECTION("assign(count, value)")
    {
        Vector v;
        v.assign(static_cast<Vector::size_type>(5), 42);
        REQUIRE(v.size() == 5);
        for (auto x : v)
            REQUIRE(x == 42);

        v.assign(static_cast<Vector::size_type>(0), 99);
        REQUIRE(v.empty());
    }
    SECTION("assign(iterator, iterator)")
    {
        int    arr[] = { 1, 2, 3, 4 };
        Vector v;
        v.assign(arr, arr + 4);
        REQUIRE(v.size() == 4);
        for (int i = 0; i < 4; ++i)
            REQUIRE(v[i] == arr[i]);

        v.assign(arr, arr); // empty range
        REQUIRE(v.empty());
    }
    SECTION("assign(initializer_list)")
    {
        Vector v;
        v.assign({ 7, 8, 9 });
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);

        v.assign({});
        REQUIRE(v.empty());
    }
    SECTION("assign replaces old contents")
    {
        Vector v = { 1, 2, 3 };
        v.assign(static_cast<Vector::size_type>(2), 5);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 5);
        REQUIRE(v[1] == 5);
    }
    SECTION("assign with 0 items")
    {
        SECTION("assign(0, value) on empty vector")
        {
            Vector v;
            v.assign(static_cast<Vector::size_type>(0), 42);
            REQUIRE(v.empty());
            REQUIRE(v.size() == 0);
            REQUIRE(v.begin() == v.end());
        }
        SECTION("assign(0, value) on non-empty vector")
        {
            Vector v                 = { 1, 2, 3, 4, 5 };
            auto   original_capacity = v.capacity();
            v.assign(static_cast<Vector::size_type>(0), 99);
            REQUIRE(v.empty());
            REQUIRE(v.size() == 0);
            REQUIRE(v.begin() == v.end());
            // I commented this out because it is not guaranteed that the capacity
            // remains the same after an assign with 0 items.
            //REQUIRE(v.capacity() >= original_capacity);
        }
        SECTION("assign(empty_range) on empty vector")
        {
            Vector v;
            int    arr[] = { 1, 2, 3 };
            v.assign(arr, arr);
            REQUIRE(v.empty());
            REQUIRE(v.size() == 0);
            REQUIRE(v.begin() == v.end());
        }
        SECTION("assign(empty_range) on non-empty vector")
        {
            Vector v                 = { 10, 20, 30 };
            auto   original_capacity = v.capacity();
            int    arr[]             = { 1, 2, 3 };
            v.assign(arr, arr);
            REQUIRE(v.empty());
            REQUIRE(v.size() == 0);
            REQUIRE(v.begin() == v.end());
            // I commented this out because it is not guaranteed that the capacity
            // remains the same after an assign with 0 items.
            //REQUIRE(v.capacity() >= original_capacity);
        }
        SECTION("assign(empty_initializer_list) on empty vector")
        {
            Vector v;
            v.assign({});
            REQUIRE(v.empty());
            REQUIRE(v.size() == 0);
            REQUIRE(v.begin() == v.end());
        }
        SECTION("assign(empty_initializer_list) on non-empty vector")
        {
            Vector v                 = { 7, 8, 9, 10 };
            auto   original_capacity = v.capacity();
            v.assign({});
            REQUIRE(v.empty());
            REQUIRE(v.size() == 0);
            REQUIRE(v.begin() == v.end());
            // I commented this out because it is not guaranteed that the capacity
            // remains the same after an assign with 0 items.
            //REQUIRE(v.capacity() >= original_capacity);
        }
    }
}

TEST_CASE("push_back() method", "[vector][push_back][modifiers]")
{
    using Vector = pw::vector<int>;

    SECTION("push_back() on empty vector")
    {
        Vector v;

        v.push_back(42);
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 42);
        REQUIRE(v.front() == 42);
        REQUIRE(v.back() == 42);
        REQUIRE(v.at(0) == 42);
    }
    SECTION("push_back() multiple elements")
    {
        Vector v;

        v.push_back(10);
        v.push_back(20);
        v.push_back(30);

        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
        REQUIRE(v.front() == 10);
        REQUIRE(v.back() == 30);
    }
    SECTION("push_back() to non-empty vector")
    {
        Vector v             = { 1, 2, 3 };
        auto   original_size = v.size();

        v.push_back(4);
        REQUIRE(v.size() == original_size + 1);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 4);
        REQUIRE(v.back() == 4);
    }
    SECTION("push_back() capacity growth")
    {
        Vector v;
        auto   original_capacity = v.capacity();

        // Add elements until capacity grows
        for (int i = 0; i < 10; ++i)
        {
            v.push_back(i);
        }

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= v.size());

        // Verify all elements are correct
        for (int i = 0; i < 10; ++i)
        {
            REQUIRE(v[i] == i);
        }
    }
    SECTION("push_back() preserves existing elements")
    {
        Vector v = { 100, 200, 300 };

        for (int i = 0; i < 50; ++i)
        {
            v.push_back(i + 1000);
        }

        REQUIRE(v.size() == 53);
        REQUIRE(v[0] == 100);
        REQUIRE(v[1] == 200);
        REQUIRE(v[2] == 300);

        for (int i = 0; i < 50; ++i)
        {
            REQUIRE(v[i + 3] == i + 1000);
        }
    }
    SECTION("push_back() with copy semantics")
    {
        Vector v;
        int    value = 99;

        v.push_back(value);
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 99);

        value = 77; // Modify original, vector should be unchanged
        REQUIRE(v[0] == 99);

        v[0] = 88; // Modify vector element, original should be unchanged
        REQUIRE(value == 77);
        REQUIRE(v[0] == 88);
    }
    SECTION("push_back() with different values")
    {
        Vector v;

        v.push_back(0);
        v.push_back(-1);
        v.push_back(2147483647);  // max int
        v.push_back(-2147483648); // min int

        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 0);
        REQUIRE(v[1] == -1);
        REQUIRE(v[2] == 2147483647);
        REQUIRE(v[3] == -2147483648);
    }
    SECTION("push_back() effects on iterators")
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
        REQUIRE(distance_after == 4);

        // Note: iterators from before push_back may be invalidated
        // We only test the new iterators
        REQUIRE(*begin_after == 1);
        REQUIRE(*(end_after - 1) == 4);
    }
    SECTION("push_back() consistency with other methods")
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
    }
    SECTION("push_back() with reserved capacity")
    {
        Vector v;
        v.reserve(100);
        auto reserved_capacity = v.capacity();

        for (int i = 0; i < 50; ++i)
        {
            v.push_back(i);
        }

        REQUIRE(v.size() == 50);
        REQUIRE(v.capacity() == reserved_capacity); // Should not reallocate

        for (int i = 0; i < 50; ++i)
        {
            REQUIRE(v[i] == i);
        }
    }
    SECTION("push_back() with different allocators")
    {
        using Allocator = basicunit::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc(5);
        Vector    v(alloc);

        v.push_back(10);
        v.push_back(20);
        v.push_back(30);

        REQUIRE(v.size() == 3);
        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
    }
    SECTION("push_back() large number of elements")
    {
        Vector    v;
        const int count = 1000;

        for (int i = 0; i < count; ++i)
        {
            v.push_back(i * 2);
        }

        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() >= v.size());

        // Verify all elements
        for (int i = 0; i < count; ++i)
        {
            REQUIRE(v[i] == i * 2);
        }

        REQUIRE(v.front() == 0);
        REQUIRE(v.back() == (count - 1) * 2);
    }
    SECTION("push_back() after clear")
    {
        Vector v                 = { 1, 2, 3, 4, 5 };
        auto   original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());

        v.push_back(100);
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 100);
        REQUIRE(v.front() == 100);
        REQUIRE(v.back() == 100);
    }
    SECTION("push_back() mixed with other operations")
    {
        Vector v;

        v.push_back(1);
        v.push_back(2);
        REQUIRE(v.size() == 2);

        v.assign({ 10, 20, 30 });
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 10);

        v.push_back(40);
        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
        REQUIRE(v[3] == 40);

        v.clear();
        v.push_back(99);
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 99);
    }
}

TEST_CASE("at() methods", "[vector][at][access]")
{
    using Vector = pw::vector<int>;

    SECTION("at() on empty vector")
    {
        Vector v;
        REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    }
    SECTION("const at() on empty vector")
    {
        const Vector v;
        REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    }
    SECTION("at() on single element vector")
    {
        Vector v = { 42 };

        REQUIRE(v.at(0) == 42);
        REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);

        v.at(0) = 99;
        REQUIRE(v.at(0) == 99);
        REQUIRE(v[0] == 99);
    }
    SECTION("const at() on single element vector")
    {
        const Vector v = { 42 };

        REQUIRE(v.at(0) == 42);
        REQUIRE_THROWS_AS(v.at(1), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    }
    SECTION("at() on multi-element vector")
    {
        Vector v = { 10, 20, 30, 40, 50 };

        REQUIRE(v.at(0) == 10);
        REQUIRE(v.at(1) == 20);
        REQUIRE(v.at(2) == 30);
        REQUIRE(v.at(3) == 40);
        REQUIRE(v.at(4) == 50);

        REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(10), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);

        v.at(2) = 333;
        REQUIRE(v.at(2) == 333);
        REQUIRE(v[2] == 333);
    }
    SECTION("const at() on multi-element vector")
    {
        const Vector v = { 10, 20, 30, 40, 50 };

        REQUIRE(v.at(0) == 10);
        REQUIRE(v.at(1) == 20);
        REQUIRE(v.at(2) == 30);
        REQUIRE(v.at(3) == 40);
        REQUIRE(v.at(4) == 50);

        REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(10), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    }
    SECTION("at() boundary conditions")
    {
        Vector v(static_cast<Vector::size_type>(1000), 7);

        REQUIRE(v.at(0) == 7);
        REQUIRE(v.at(999) == 7);
        REQUIRE_THROWS_AS(v.at(1000), std::out_of_range);

        v.at(0)   = 1;
        v.at(999) = 2;
        REQUIRE(v.at(0) == 1);
        REQUIRE(v.at(999) == 2);
        REQUIRE(v.at(500) == 7);
    }
    SECTION("at() return type and reference semantics")
    {
        Vector v   = { 100 };

        int&   ref = v.at(0);
        ref        = 200;
        REQUIRE(v.at(0) == 200);
        REQUIRE(v[0] == 200);

        const Vector& cv   = v;
        const int&    cref = cv.at(0);
        REQUIRE(cref == 200);
    }
}

TEST_CASE("operator[] methods", "[vector][operator][access]")
{
    using Vector = pw::vector<int>;

    SECTION("operator[] on single element vector")
    {
        Vector v = { 42 };

        REQUIRE(v[0] == 42);

        v[0] = 99;
        REQUIRE(v[0] == 99);
        REQUIRE(v.at(0) == 99);
    }
    SECTION("const operator[] on single element vector")
    {
        const Vector v = { 42 };

        REQUIRE(v[0] == 42);
    }
    SECTION("operator[] on multi-element vector")
    {
        Vector v = { 10, 20, 30, 40, 50 };

        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
        REQUIRE(v[3] == 40);
        REQUIRE(v[4] == 50);

        v[2] = 333;
        REQUIRE(v[2] == 333);
        REQUIRE(v.at(2) == 333);
    }
    SECTION("const operator[] on multi-element vector")
    {
        const Vector v = { 10, 20, 30, 40, 50 };

        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
        REQUIRE(v[3] == 40);
        REQUIRE(v[4] == 50);
    }
    SECTION("operator[] boundary conditions")
    {
        Vector v(static_cast<Vector::size_type>(1000), 7);

        REQUIRE(v[0] == 7);
        REQUIRE(v[999] == 7);

        v[0]   = 1;
        v[999] = 2;
        REQUIRE(v[0] == 1);
        REQUIRE(v[999] == 2);
        REQUIRE(v[500] == 7);
    }
    SECTION("operator[] return type and reference semantics")
    {
        Vector v   = { 100 };

        int&   ref = v[0];
        ref        = 200;
        REQUIRE(v[0] == 200);
        REQUIRE(v.at(0) == 200);

        const Vector& cv   = v;
        const int&    cref = cv[0];
        REQUIRE(cref == 200);
    }
    SECTION("operator[] vs at() comparison")
    {
        Vector v = { 1, 2, 3 };

        for (Vector::size_type i = 0; i < v.size(); ++i)
        {
            REQUIRE(v[i] == v.at(i));
        }

        v[1] = 999;
        REQUIRE(v[1] == v.at(1));
        REQUIRE(v[1] == 999);
    }
}

TEST_CASE("front() and back() methods", "[vector][front][back][access]")
{
    using Vector = pw::vector<int>;

    SECTION("front() and back() on single element vector")
    {
        Vector v = { 42 };

        REQUIRE(v.front() == 42);
        REQUIRE(v.back() == 42);
        REQUIRE(v.front() == v.back());

        v.front() = 99;
        REQUIRE(v.front() == 99);
        REQUIRE(v.back() == 99);
        REQUIRE(v[0] == 99);

        v.back() = 77;
        REQUIRE(v.front() == 77);
        REQUIRE(v.back() == 77);
        REQUIRE(v[0] == 77);
    }
    SECTION("const front() and back() on single element vector")
    {
        const Vector v = { 42 };

        REQUIRE(v.front() == 42);
        REQUIRE(v.back() == 42);
        REQUIRE(v.front() == v.back());
    }
    SECTION("front() and back() on multi-element vector")
    {
        Vector v = { 10, 20, 30, 40, 50 };

        REQUIRE(v.front() == 10);
        REQUIRE(v.back() == 50);
        REQUIRE(v.front() == v[0]);
        REQUIRE(v.back() == v[4]);
        REQUIRE(v.back() == v[v.size() - 1]);

        v.front() = 100;
        REQUIRE(v.front() == 100);
        REQUIRE(v[0] == 100);
        REQUIRE(v.back() == 50);

        v.back() = 500;
        REQUIRE(v.back() == 500);
        REQUIRE(v[4] == 500);
        REQUIRE(v.front() == 100);
    }
    SECTION("const front() and back() on multi-element vector")
    {
        const Vector v = { 10, 20, 30, 40, 50 };

        REQUIRE(v.front() == 10);
        REQUIRE(v.back() == 50);
        REQUIRE(v.front() == v[0]);
        REQUIRE(v.back() == v[4]);
        REQUIRE(v.back() == v[v.size() - 1]);
    }
    SECTION("front() and back() return type and reference semantics")
    {
        Vector v         = { 100, 200 };

        int&   front_ref = v.front();
        int&   back_ref  = v.back();

        front_ref        = 1000;
        back_ref         = 2000;

        REQUIRE(v.front() == 1000);
        REQUIRE(v.back() == 2000);
        REQUIRE(v[0] == 1000);
        REQUIRE(v[1] == 2000);

        const Vector& cv              = v;
        const int&    const_front_ref = cv.front();
        const int&    const_back_ref  = cv.back();

        REQUIRE(const_front_ref == 1000);
        REQUIRE(const_back_ref == 2000);
    }
    SECTION("front() and back() consistency with other access methods")
    {
        Vector v = { 7, 8, 9, 10, 11 };

        REQUIRE(v.front() == v[0]);
        REQUIRE(v.front() == v.at(0));
        REQUIRE(v.front() == *v.begin());

        REQUIRE(v.back() == v[v.size() - 1]);
        REQUIRE(v.back() == v.at(v.size() - 1));
        REQUIRE(v.back() == *(v.end() - 1));

        v.front() = 77;
        v.back()  = 111;

        REQUIRE(v[0] == 77);
        REQUIRE(v.at(0) == 77);
        REQUIRE(*v.begin() == 77);

        REQUIRE(v[v.size() - 1] == 111);
        REQUIRE(v.at(v.size() - 1) == 111);
        REQUIRE(*(v.end() - 1) == 111);
    }
    SECTION("front() and back() on large vector")
    {
        Vector v(static_cast<Vector::size_type>(1000), 5);

        REQUIRE(v.front() == 5);
        REQUIRE(v.back() == 5);

        v.front() = 1;
        v.back()  = 1000;

        REQUIRE(v.front() == 1);
        REQUIRE(v.back() == 1000);
        REQUIRE(v[0] == 1);
        REQUIRE(v[999] == 1000);
        REQUIRE(v[500] == 5);
    }
}

TEST_CASE("data() method", "[vector][data][access]")
{
    using Vector = pw::vector<int>;

    SECTION("data() on empty vector")
    {
        Vector v;
        REQUIRE(v.data() == nullptr);

        const Vector& cv = v;
        REQUIRE(cv.data() == nullptr);
    }
    SECTION("data() on single element vector")
    {
        Vector v   = { 42 };

        int*   ptr = v.data();
        REQUIRE(ptr != nullptr);
        REQUIRE(*ptr == 42);
        REQUIRE(ptr == &v[0]);
        REQUIRE(ptr == &v.front());
        REQUIRE(ptr == &v.back());

        *ptr = 99;
        REQUIRE(v[0] == 99);
        REQUIRE(v.front() == 99);
    }
    SECTION("const data() on single element vector")
    {
        const Vector v   = { 42 };

        const int*   ptr = v.data();
        REQUIRE(ptr != nullptr);
        REQUIRE(*ptr == 42);
        REQUIRE(ptr == &v[0]);
        REQUIRE(ptr == &v.front());
        REQUIRE(ptr == &v.back());
    }
    SECTION("data() on multi-element vector")
    {
        Vector v   = { 10, 20, 30, 40, 50 };

        int*   ptr = v.data();
        REQUIRE(ptr != nullptr);
        REQUIRE(ptr == &v[0]);
        REQUIRE(ptr == &v.front());

        for (Vector::size_type i = 0; i < v.size(); ++i)
        {
            REQUIRE(ptr[i] == v[i]);
            REQUIRE(&ptr[i] == &v[i]);
        }

        ptr[2] = 333;
        REQUIRE(v[2] == 333);
        REQUIRE(v.at(2) == 333);
    }
    SECTION("const data() on multi-element vector")
    {
        const Vector v   = { 10, 20, 30, 40, 50 };

        const int*   ptr = v.data();
        REQUIRE(ptr != nullptr);
        REQUIRE(ptr == &v[0]);
        REQUIRE(ptr == &v.front());

        for (Vector::size_type i = 0; i < v.size(); ++i)
        {
            REQUIRE(ptr[i] == v[i]);
            REQUIRE(&ptr[i] == &v[i]);
        }
    }
    SECTION("data() pointer arithmetic")
    {
        Vector v   = { 1, 2, 3, 4, 5 };

        int*   ptr = v.data();
        REQUIRE(ptr != nullptr);

        REQUIRE(ptr == &v.front());
        REQUIRE(ptr + v.size() - 1 == &v.back());
        REQUIRE(*(ptr + 2) == v[2]);
        REQUIRE(*(ptr + 4) == v[4]);

        for (Vector::size_type i = 0; i < v.size(); ++i)
        {
            REQUIRE(*(ptr + i) == v[i]);
        }
    }
    SECTION("data() consistency with iterators")
    {
        Vector v   = { 7, 8, 9, 10, 11 };

        int*   ptr = v.data();
        REQUIRE(ptr != nullptr);
        REQUIRE(ptr == &(*v.begin()));

        auto it = v.begin();
        for (Vector::size_type i = 0; i < v.size(); ++i, ++it)
        {
            REQUIRE(ptr + i == &(*it));
            REQUIRE(*(ptr + i) == *it);
        }
    }
    SECTION("data() on large vector")
    {
        Vector v(static_cast<Vector::size_type>(1000), 7);

        int*   ptr = v.data();
        REQUIRE(ptr != nullptr);
        REQUIRE(ptr == &v[0]);
        REQUIRE(ptr == &v.front());
        REQUIRE(ptr + 999 == &v.back());

        REQUIRE(*ptr == 7);
        REQUIRE(*(ptr + 999) == 7);
        REQUIRE(*(ptr + 500) == 7);

        *(ptr + 500) = 555;
        REQUIRE(v[500] == 555);
        REQUIRE(v.at(500) == 555);
    }
    SECTION("data() after vector operations")
    {
        Vector v = { 1, 2, 3 };

        v.clear();
        // After clear, data() should return nullptr for empty vector
        REQUIRE(v.data() == nullptr);

        v.assign({ 10, 20 });
        int* after_assign_ptr = v.data();
        REQUIRE(after_assign_ptr != nullptr);
        REQUIRE(*after_assign_ptr == 10);
        REQUIRE(*(after_assign_ptr + 1) == 20);
    }
    SECTION("data() return type verification")
    {
        Vector v = { 100, 200 };

        static_assert(std::is_same_v<decltype(v.data()), int*>);

        const Vector& cv = v;
        static_assert(std::is_same_v<decltype(cv.data()), const int*>);

        int*       mutable_ptr = v.data();
        const int* const_ptr   = cv.data();

        REQUIRE(mutable_ptr == const_ptr);
        REQUIRE(*mutable_ptr == *const_ptr);
    }
}

TEST_CASE("clear() method", "[vector][clear][modifiers]")
{
    using Vector = pw::vector<int>;

    SECTION("clear() on empty vector")
    {
        Vector v;
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.data() == nullptr);
    }
    SECTION("clear() on single element vector")
    {
        Vector v = { 42 };
        REQUIRE(!v.empty());
        REQUIRE(v.size() == 1);
        auto original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.data() == nullptr);
        REQUIRE(v.capacity() == original_capacity);
    }
    SECTION("clear() on multi-element vector")
    {
        Vector v = { 10, 20, 30, 40, 50 };
        REQUIRE(!v.empty());
        REQUIRE(v.size() == 5);
        auto original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.data() == nullptr);
        REQUIRE(v.capacity() == original_capacity);
    }
    SECTION("clear() preserves capacity")
    {
        Vector v;
        v.reserve(100);
        auto reserved_capacity = v.capacity();
        REQUIRE(reserved_capacity >= 100);

        v.assign({ 1, 2, 3, 4, 5 });
        REQUIRE(v.size() == 5);

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
    }
    SECTION("clear() on large vector")
    {
        Vector v(static_cast<Vector::size_type>(1000), 7);
        REQUIRE(v.size() == 1000);
        REQUIRE(!v.empty());
        auto original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.data() == nullptr);
        REQUIRE(v.capacity() == original_capacity);
    }
    SECTION("clear() multiple times")
    {
        Vector v                 = { 1, 2, 3 };
        auto   original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == original_capacity);

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == original_capacity);

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == original_capacity);
    }
    SECTION("clear() then reuse vector")
    {
        Vector v                 = { 1, 2, 3, 4, 5 };
        auto   original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == original_capacity);

        v.assign({ 10, 20 });
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
    }
    SECTION("clear() with different allocators")
    {
        using Allocator = basicunit::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc(5);
        Vector    v({ 1, 2, 3 }, alloc);
        auto      original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == original_capacity);
        REQUIRE(v.get_allocator() == alloc);
    }
    SECTION("clear() effects on iterators")
    {
        Vector v            = { 1, 2, 3, 4 };

        auto   begin_before = v.begin();
        auto   end_before   = v.end();
        REQUIRE(begin_before != end_before);

        v.clear();

        auto begin_after = v.begin();
        auto end_after   = v.end();
        REQUIRE(begin_after == end_after);

        // Note: iterators from before clear() are now invalid
        // We only test the new iterators
    }
    SECTION("clear() followed by access methods")
    {
        Vector v = { 1, 2, 3 };

        v.clear();
        REQUIRE(v.empty());

        // These should throw because vector is empty
        REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
        // Note: front(), back() have undefined behavior on empty vector
        // so we don't test them here

        REQUIRE(v.data() == nullptr);
        REQUIRE(v.begin() == v.end());
    }
}

TEST_CASE("shrink_to_fit() method", "[vector][shrink_to_fit][capacity]")
{
    using Vector = pw::vector<int>;

    SECTION("shrink_to_fit() on empty vector")
    {
        Vector v;
        REQUIRE(v.empty());
        auto original_capacity = v.capacity();

        v.shrink_to_fit();
        REQUIRE(v.empty());
        REQUIRE(v.capacity() <= original_capacity);
        REQUIRE(v.begin() == v.end());
    }
    SECTION("shrink_to_fit() when capacity equals size")
    {
        Vector v = { 1, 2, 3 };
        // Force capacity to equal size (implementation dependent)
        auto original_size     = v.size();
        auto original_capacity = v.capacity();

        v.shrink_to_fit();
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("shrink_to_fit() after reserve")
    {
        Vector v = { 1, 2, 3, 4, 5 };
        v.reserve(100);
        auto size_before     = v.size();
        auto capacity_before = v.capacity();
        REQUIRE(capacity_before >= 100);
        REQUIRE(capacity_before > size_before);

        v.shrink_to_fit();
        REQUIRE(v.size() == size_before);
        REQUIRE(v.capacity() <= capacity_before);
        REQUIRE(v.capacity() >= v.size());

        // Verify elements are preserved
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 4);
        REQUIRE(v[4] == 5);
    }
    SECTION("shrink_to_fit() after clear")
    {
        Vector v;
        v.reserve(50);
        v.assign({ 1, 2, 3 });
        v.clear();

        auto capacity_before = v.capacity();
        REQUIRE(v.empty());
        REQUIRE(capacity_before > 0);

        v.shrink_to_fit();
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() <= capacity_before);
        // Note: capacity might be 0 or minimal for empty vector
    }
    SECTION("shrink_to_fit() multiple times")
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
        REQUIRE(v.size() == 3);
        REQUIRE(capacity_after_second == capacity_after_first);

        // Elements should be preserved
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("shrink_to_fit() preserves data integrity")
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
        REQUIRE(v.size() == 100);
        REQUIRE(v.capacity() <= capacity_before);
        REQUIRE(v.capacity() >= v.size());

        // Verify all elements are preserved and correct
        for (int i = 0; i < 100; ++i)
        {
            REQUIRE(v[i] == i * 2);
        }

        // Verify iterators work
        auto it = v.begin();
        for (int i = 0; i < 100; ++i, ++it)
        {
            REQUIRE(*it == i * 2);
        }
        REQUIRE(it == v.end());
    }
    SECTION("shrink_to_fit() effects on pointers and iterators")
    {
        Vector v;
        v.reserve(100);
        v.assign({ 10, 20, 30 });

        auto old_data  = v.data();
        auto old_begin = v.begin();
        auto old_end   = v.end();

        v.shrink_to_fit();

        // After shrink_to_fit, pointers and iterators may be invalidated
        // We can only test that the new ones work correctly
        REQUIRE(v.size() == 3);
        REQUIRE(v.data() != nullptr);
        REQUIRE(v.begin() != v.end());
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
        REQUIRE(*v.begin() == 10);
        REQUIRE(*(v.end() - 1) == 30);
    }
    SECTION("shrink_to_fit() with different allocators")
    {
        using Allocator = basicunit::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc(5);
        Vector    v(alloc);
        v.assign({ 1, 2, 3 });
        v.reserve(50);

        auto capacity_before = v.capacity();
        REQUIRE(capacity_before >= 50);

        v.shrink_to_fit();
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() <= capacity_before);
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("shrink_to_fit() is non-binding request")
    {
        Vector v                 = { 1, 2, 3 };
        auto   original_capacity = v.capacity();

        v.shrink_to_fit();

        // shrink_to_fit is a non-binding request
        // We can only verify that capacity is reasonable
        REQUIRE(v.capacity() >= v.size());
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
}

TEST_CASE("reserve() method", "[vector][reserve][capacity]")
{
    using Vector = pw::vector<int>;

    SECTION("reserve() on empty vector")
    {
        Vector v;
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        auto original_capacity = v.capacity();

        v.reserve(10);
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 10);
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.data() == nullptr);
    }
    SECTION("reserve() zero capacity")
    {
        Vector v                 = { 1, 2, 3 };
        auto   original_size     = v.size();
        auto   original_capacity = v.capacity();

        v.reserve(0);
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= original_capacity);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("reserve() less than current capacity")
    {
        Vector v                 = { 1, 2, 3, 4, 5 };
        auto   original_size     = v.size();
        auto   original_capacity = v.capacity();

        v.reserve(original_capacity - 1);
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= original_capacity);

        // Elements should be preserved
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 4);
        REQUIRE(v[4] == 5);
    }
    SECTION("reserve() equal to current capacity")
    {
        Vector v                 = { 1, 2, 3 };
        auto   original_size     = v.size();
        auto   original_capacity = v.capacity();

        v.reserve(original_capacity);
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= original_capacity);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("reserve() greater than current capacity")
    {
        Vector v                 = { 1, 2, 3 };
        auto   original_size     = v.size();
        auto   original_capacity = v.capacity();
        auto   new_capacity      = original_capacity + 50;

        v.reserve(new_capacity);
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= new_capacity);

        // Elements should be preserved
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("reserve() large capacity")
    {
        Vector v = { 10, 20, 30 };
        v.reserve(1000);

        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() >= 1000);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);

        // Should be able to add many elements without reallocation
        auto capacity_after_reserve = v.capacity();
        for (int i = 0; i < 500; ++i)
        {
            v.push_back(i);
        }
        REQUIRE(v.capacity() == capacity_after_reserve);
        REQUIRE(v.size() == 503);
    }
    SECTION("reserve() multiple times")
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
        REQUIRE(v.capacity() >= capacity_after_second);
        REQUIRE(v.size() == 2);

        // Elements should be preserved throughout
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
    }
    SECTION("reserve() preserves data integrity")
    {
        Vector v;
        for (int i = 0; i < 50; ++i)
        {
            v.push_back(i * 3);
            REQUIRE(v[i] == i * 3);
        }

        auto original_size = v.size();
        v.reserve(500);

        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= 500);

        // Verify all elements are preserved
        for (int i = 0; i < 50; ++i)
        {
            REQUIRE(v[i] == i * 3);
        }

        // Verify iterators work correctly
        auto it = v.begin();
        for (int i = 0; i < 50; ++i, ++it)
        {
            REQUIRE(*it == i * 3);
        }
        REQUIRE(it == v.end());
    }
    SECTION("reserve() with different allocators")
    {
        using Allocator = basicunit::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc(5);
        Vector    v({ 1, 2, 3 }, alloc);
        auto      original_size = v.size();

        v.reserve(25);
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= 25);
        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
    SECTION("reserve() effects on pointers and iterators")
    {
        Vector v                 = { 10, 20, 30 };
        auto   original_capacity = v.capacity();

        auto   old_data          = v.data();
        auto   old_begin         = v.begin();
        auto   old_end           = v.end();

        // Reserve significantly more than current capacity
        v.reserve(original_capacity + 100);

        // After reserve that increases capacity, pointers/iterators may be invalidated
        // We can only test that the new ones work correctly
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
    SECTION("reserve() followed by operations")
    {
        Vector v;
        v.reserve(100);
        auto reserved_capacity = v.capacity();

        // Add elements up to reserved capacity
        for (int i = 0; i < 50; ++i)
        {
            v.push_back(i);
        }

        REQUIRE(v.size() == 50);
        REQUIRE(v.capacity() == reserved_capacity);

        // Clear and reuse
        v.clear();
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == reserved_capacity);

        // Add different elements
        v.assign({ 100, 200, 300 });
        REQUIRE(v.size() == 3);
        //REQUIRE(v.capacity() == reserved_capacity);
        REQUIRE(v[0] == 100);
        REQUIRE(v[1] == 200);
        REQUIRE(v[2] == 300);
    }
    SECTION("reserve() with max_size() considerations")
    {
        Vector v;

        // Reserve a reasonable amount (much less than max_size)
        v.reserve(1000);
        REQUIRE(v.capacity() >= 1000);
        REQUIRE(v.empty());

        // Note: Testing reserve(max_size()) or larger would be
        // implementation-dependent and might throw std::length_error
        // We avoid testing edge cases that might exhaust memory
    }
    SECTION("reserve() maintains strong exception safety")
    {
        Vector v                 = { 1, 2, 3, 4, 5 };
        auto   original_size     = v.size();
        auto   original_capacity = v.capacity();

        // Reserve a reasonable amount that should succeed
        v.reserve(original_capacity + 10);

        // If reserve succeeded, all elements should be preserved
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() >= original_capacity + 10);
        for (int i = 0; i < 5; ++i)
        {
            REQUIRE(v[i] == i + 1);
        }
    }
}

TEST_CASE("resize() method", "[vector][resize][modifiers]")
{
    using Vector = pw::vector<int>;

    SECTION("resize() on empty vector")
    {
        Vector v;
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);

        v.resize(5);
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
        REQUIRE(!v.empty());

        for (Vector::size_type i = 0; i < 5; ++i)
        {
            REQUIRE(v[i] == 0);
        }
    }
    SECTION("resize() to zero on non-empty vector")
    {
        Vector v                 = { 1, 2, 3, 4, 5 };
        auto   original_capacity = v.capacity();

        v.resize(0);
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == original_capacity);
        REQUIRE(v.begin() == v.end());
    }
    SECTION("resize() to same size")
    {
        Vector v                 = { 10, 20, 30 };
        auto   original_size     = v.size();
        auto   original_capacity = v.capacity();

        v.resize(original_size);
        REQUIRE(v.size() == original_size);
        REQUIRE(v.capacity() == original_capacity);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
    }
    SECTION("resize() to larger size")
    {
        Vector v             = { 1, 2, 3 };
        auto   original_size = v.size();

        v.resize(7);
        REQUIRE(v.size() == 7);
        REQUIRE(v.capacity() >= 7);

        // Original elements preserved
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);

        // New elements default-constructed (0 for int)
        REQUIRE(v[3] == 0);
        REQUIRE(v[4] == 0);
        REQUIRE(v[5] == 0);
        REQUIRE(v[6] == 0);
    }
    SECTION("resize() to smaller size")
    {
        Vector v                 = { 10, 20, 30, 40, 50 };
        auto   original_capacity = v.capacity();

        v.resize(3);
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == original_capacity);

        // Remaining elements preserved
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
    }
    SECTION("resize() within existing capacity")
    {
        Vector v { 1, 2, 3, 4, 5 };
        v.reserve(100);
        auto reserved_capacity = v.capacity();

        v.resize(50);
        REQUIRE(v.size() == 50);
        REQUIRE(v.capacity() == reserved_capacity);

        // Original elements preserved
        for (int i = 0; i < 5; ++i)
        {
            REQUIRE(v[i] == i + 1);
        }

        // New elements default-constructed
        for (Vector::size_type i = 5; i < 50; ++i)
        {
            REQUIRE(v[i] == 0);
        }
    }
    SECTION("resize() beyond existing capacity")
    {
        Vector v                 = { 1, 2, 3 };
        auto   original_capacity = v.capacity();

        v.resize(original_capacity + 10);
        REQUIRE(v.size() == original_capacity + 10);
        REQUIRE(v.capacity() > original_capacity);

        // Original elements preserved
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);

        // New elements default-constructed
        for (Vector::size_type i = 3; i < v.size(); ++i)
        {
            REQUIRE(v[i] == 0);
        }
    }
    SECTION("resize() multiple times")
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
        REQUIRE(v.size() == 8);
        for (Vector::size_type i = 0; i < 8; ++i)
        {
            REQUIRE(v[i] == 0);
        }
    }
    SECTION("resize() with different allocators")
    {
        using Allocator = basicunit::allocator_move_assignment<int>;
        using Vector    = pw::vector<int, Allocator>;

        Allocator alloc(5);
        Vector    v({ 1, 2, 3 }, alloc);

        v.resize(6);
        REQUIRE(v.size() == 6);
        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 0);
        REQUIRE(v[4] == 0);
        REQUIRE(v[5] == 0);

        v.resize(2);
        REQUIRE(v.size() == 2);
        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
    }
    SECTION("resize() preserves data integrity")
    {
        Vector v;
        for (int i = 0; i < 20; ++i)
        {
            v.push_back(i * 10);
        }

        v.resize(30);
        REQUIRE(v.size() == 30);

        // Original elements preserved
        for (int i = 0; i < 20; ++i)
        {
            REQUIRE(v[i] == i * 10);
        }

        // New elements default-constructed
        for (Vector::size_type i = 20; i < 30; ++i)
        {
            REQUIRE(v[i] == 0);
        }

        v.resize(15);
        REQUIRE(v.size() == 15);

        // Remaining elements preserved
        for (int i = 0; i < 15; ++i)
        {
            REQUIRE(v[i] == i * 10);
        }
    }
    SECTION("resize() effects on iterators")
    {
        Vector v         = { 10, 20, 30 };

        auto   old_begin = v.begin();
        auto   old_end   = v.end();
        REQUIRE(old_end - old_begin == 3);

        v.resize(5);

        auto new_begin = v.begin();
        auto new_end   = v.end();
        REQUIRE(new_end - new_begin == 5);

        // Note: old iterators may be invalidated
        // We only test the new iterators
        REQUIRE(*new_begin == 10);
        REQUIRE(*(new_begin + 1) == 20);
        REQUIRE(*(new_begin + 2) == 30);
        REQUIRE(*(new_begin + 3) == 0);
        REQUIRE(*(new_begin + 4) == 0);
    }
    SECTION("resize() consistency with other methods")
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
        REQUIRE(v.size() == 6);
        REQUIRE(v.front() == 0);
        REQUIRE(v.back() == 0);
        REQUIRE(v[1] == 100);
        REQUIRE(v[2] == 200);
        REQUIRE(v[4] == 0);
        REQUIRE(v[5] == 0);
    }
    SECTION("resize() after clear")
    {
        Vector v                 = { 1, 2, 3, 4, 5 };
        auto   original_capacity = v.capacity();

        v.clear();
        REQUIRE(v.empty());

        v.resize(3);
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() >= original_capacity);
        REQUIRE(v[0] == 0);
        REQUIRE(v[1] == 0);
        REQUIRE(v[2] == 0);
    }
    SECTION("resize() large size")
    {
        Vector v;

        v.resize(1000);
        REQUIRE(v.size() == 1000);
        REQUIRE(v.capacity() >= 1000);

        // Verify all elements are default-constructed
        for (Vector::size_type i = 0; i < 1000; ++i)
        {
            CAPTURE(i);
            REQUIRE(v[i] == 0);
        }

        // Verify front and back
        REQUIRE(v.front() == 0);
        REQUIRE(v.back() == 0);
    }
    SECTION("resize() alternating sizes")
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
    }
}
