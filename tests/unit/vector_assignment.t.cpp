#include <pw/algorithm>
#include <pw/impl/move.h>
#include <pw/vector>
#include <test_allocator_base.h>
#include <test_allocator_copy_assignment.h>
#include <test_allocator_move_assignment.h>
#include <test_allocator_swapable.h>
#include <test_testtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

/*
 * Type Requirements:
 * - No extra
 */
TEMPLATE_LIST_TEST_CASE("Assignment operator", "[vector][operator=]", pw::test::TestTypeListInt)
{
    using Vector = TestType;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("operator=(const_ref) both empty")
        {
            Vector op2;
            op2 = v;
            THEN("equal")
            {
                REQUIRE(v == op2);
            }
        }
        WHEN("operator=(const_ref) lhs has elements")
        {
            Vector op2 { 1, 2 };
            op2 = v;
            THEN("equal")
            {
                REQUIRE(v == op2);
            }
        }
        WHEN("operator=(const_ref) rhs has elements")
        {
            Vector op2 { 1, 2 };
            v = op2;
            THEN("they  are same")
            {
                REQUIRE(v == op2);
            }
        }
        WHEN("operator=(move) both empty")
        {
            Vector op2;
            op2 = pw::move(v);
            THEN("they are same")
            {
                REQUIRE(v == op2);
            }
        }
        WHEN("operator=(move) lhs has elements")
        {
            Vector op2 { 1, 2 };
            op2 = pw::move(v);
            THEN("they are same")
            {
                REQUIRE(v == op2);
            }
        }
        WHEN("operator=(move) rhs has elements")
        {
            Vector expected { 1, 2 };
            Vector op2 { 1, 2 };
            v = pw::move(op2);
            THEN("equal")
            {
                REQUIRE(v == expected);
            }
        }
        WHEN("operator=(initializer_list) lhs is empty")
        {
            Vector expected { 10, 11, 12 };
            v = { 10, 11, 12 };

            THEN("equal")
            {
                REQUIRE(v == expected);
            }
        }
        WHEN("operator=(initializer_list) lhs has fewer")
        {
            Vector expected { 1, 2, 3 };
            v.resize(2);
            v = { 1, 2, 3 };

            THEN("equal")
            {
                REQUIRE(v == expected);
            }
        }
        WHEN("operator=(initializer_list) rhs has fewer")
        {
            Vector expected { 1, 2, 3 };
            v.resize(5);
            v = { 1, 2, 3 };

            THEN("equal")
            {
                REQUIRE(v == expected);
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
                     << " v1.copy_allocator().m_instance = " << v1.get_allocator().m_instance
                     << " v2.copy_allocator().m_instance = " << v2.get_allocator().m_instance);
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

SCENARIO("Assignment operator", "[vector][operator=]")
{
    using Vector = pw::vector<int, pw::test::allocator_base<int>>;
    REQUIRE(!pw::allocator_traits<Vector::allocator_type>::propagate_on_container_move_assignment::value);
    GIVEN("A vector with allocator move assignment false")
    {
        WHEN("operator=(const_ref) lhs.size() < rhs.size() but less allocated")
        {
            Vector lhs { 1, 2 };
            Vector rhs { 1, 2, 3 };

            lhs = rhs;
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
        WHEN("operator=(const_ref) lhs.size() < rhs.size() but lhs allocated")
        {
            Vector lhs { 1, 2 };
            Vector rhs { 1, 2, 3 };
            lhs.reserve(rhs.size());
            lhs = rhs;
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
        WHEN("operator=(move) lhs.size() < rhs.size() but lhs allocated")
        {
            Vector lhs { 1, 2 };
            Vector rhs { 1, 2, 3 };
            lhs.reserve(rhs.size());
            lhs = pw::move(rhs);
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
    }
}