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
            THEN("begin() is null")
            {
                REQUIRE(!v.begin());
            }
            THEN("end() is null")
            {
                REQUIRE(!v.end());
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
                REQUIRE(v.begin());
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(v.end());
                REQUIRE(*(v.end() - 1) == value);
            }
            THEN("all elements have correct value")
            {
                for (typename Vector::size_type i = 0; i < total; ++i) {
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
        auto original_size = v1.size();
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
        REQUIRE(v[count-1] == value);
        for (Vector::size_type i = 0; i < count; ++i) {
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
        Vector v1(static_cast<Vector::size_type>(0), value, alloc);
        
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
        Vector v1(&d[0], &d[0], alloc);  // first == last
        
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
            Vector v{1, 2, 3};
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
            v1 = {4, 5, 6};
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
