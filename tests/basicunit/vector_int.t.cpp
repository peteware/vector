#include "basicunit_allocator.h"

#include <pw/vector>

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
        Vector v;
        WHEN("Nothing changes")
        {
            THEN("empty() is true") { REQUIRE(v.empty()); }
            THEN("size() is 0") { REQUIRE(0 == v.size()); }
            THEN("begin() is null") { REQUIRE(!v.begin()); }
            THEN("end() is null") { REQUIRE(!v.end()); }
        }
    }
    GIVEN("A vector of count values")
    {
        typename Vector::size_type const total = 10;
        value_type const                 value = 3;
        Vector                           v(total, value);

        WHEN("Initialized with value")
        {
            THEN("empty() is false") { REQUIRE(!v.empty()); }
            THEN("size() is total") { REQUIRE(total == v.size()); }
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
        }
    }
}

TEST_CASE("Constructors use allocator", "[constructor][allocator]")
{
    using Allocator  = basicunit::my_allocator<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = typename Vector::value_type;

    Allocator alloc;
    SECTION("Default constructor with allocator")
    {
        // constexpr explicit vector(allocator_type const& alloc) noexcept;
        Vector v(alloc);

        REQUIRE(v.get_allocator() == alloc);
    }
    SECTION("count and value with allocator")
    {
        // constexpr vector(size_type count, value_type const& value, allocator_type const& alloc = allocator_type());
        Vector::size_type const count = 30;
        value_type const        value = 812;
        Vector                  v(count, value, alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == count);
        REQUIRE(v[0] == value);
    }
    SECTION("count with allocator")
    {
        // constexpr explicit vector(size_type count, allocator_type const& alloc = allocator_type());
        Vector::size_type const count = 30;
        Vector                  v(count, alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == count);
    }
    SECTION("Copy constructor with allocator")
    {
        // constexpr vector(vector const& other, allocator_type const& alloc);
        Allocator alloc2;
        Vector    v1(alloc);
        Vector    v2(v1, alloc2);

        REQUIRE(v2.get_allocator() == alloc2);
    }
    SECTION("Move constructor with allocator")
    {
        // constexpr vector(vector&& other, allocator_type const& alloc);
        Allocator alloc2;
        Vector    v1(alloc);
        Vector    v2(pw::move(v1), alloc2);

        REQUIRE(v2.get_allocator() == alloc2);
    }
    SECTION("Initializer list with allocator")
    {
        // constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());
        Vector v({ 1, 2, 3, 4 }, alloc);

        REQUIRE(v.get_allocator() == alloc);
        // REQUIRE(v.size() == 4);
        // REQUIRE(v[0] == 1);
    }
    SECTION("Construct from iterator with allocator")
    {
        // template<class Iterator>
        // constexpr vector(Iterator first, Iterator last, allocator_type const& alloc = allocator_type());
        value_type d[3] = { 23, 2, 1 };
        Vector     v(&d[0], &d[3], alloc);

        REQUIRE(v.get_allocator() == alloc);
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 23);
    }
}

TEST_CASE("Copy Assignment uses allocator", "[assignment][allocator][copy]")
{
    // constexpr vector& operator=(const vector& other);
    //     using propagate_on_container_copy_assignment = false_type;
    //     using propagate_on_container_copy_assignment = true_type;
}

TEST_CASE("Move Assignment use allocator", "[assignment][allocator][move]")
{
    // constexpr vector& operator=(vector&& other) noexcept(
    //     using propagate_on_container_move_assignment = false_type;
    //     using propagate_on_container_move_assignment = true_type;
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
}

TEST_CASE("Constructors with Allocator", "[vector][constructor][allocator]")
{
    using Allocator  = basicunit::my_allocator<int>;
    using Vector     = pw::vector<int, Allocator>;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector with allocator")
    {
        Allocator allocator;
        Vector    v(allocator);
        WHEN("Check allocator")
        {
            THEN("allocator is same") { REQUIRE(allocator == v.get_allocator()); }
        }
    }
    GIVEN("Two empty vectors with allocator")
    {
        Vector v1;
        Vector v2;
        WHEN("Default allocator")
        {
            THEN("allocators are different") { REQUIRE(!(v1.get_allocator() == v2.get_allocator())); }
        }
    }
    GIVEN("Vector with count, value, and allocator")
    {
        typename Vector::size_type const total = 10;
        Allocator                        allocator;
        value_type const                 value = 3;
        Vector                           v(total, value, allocator);

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
            THEN("the get_allocator() returns origal instance") { REQUIRE(allocator == v.get_allocator()); }
        }
    }
}
