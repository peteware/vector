#include <pw/vector>

#include <pw/impl/move.h>
#include <test_allocator_base.h>
#include <test_allocator_copy_assignment.h>
#include <test_allocator_move_assignment.h>
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
    // - op1.size() < op2.size() && op1.capacity() > op2.capacity()
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
    // - op1.size() < op2.size() && op1.capacity() > op2.capacity()
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

SCENARIO("Assignment operator", "[vector][operator=]")
{
    using Vector = pw::vector<int, pw::test::allocator_base<int>>;
    REQUIRE(!pw::allocator_traits<Vector::allocator_type>::propagate_on_container_move_assignment::value);
    GIVEN("A vector with allocator move assignment false")
    {
        WHEN("operator=(const_ref) lhs.size() < rhs.size() but less capacity")
        {
            Vector lhs { 1, 2 };
            Vector rhs { 1, 2, 3 };

            lhs = rhs;
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
        WHEN("operator=(const_ref) lhs.size() < rhs.size() but lhs capacity")
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
        WHEN("operator=(move) lhs.size() < rhs.size() but lhs capacity")
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