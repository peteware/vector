#include "test_base_allocator.h"

#include <test_testtype.h>

#include <pw/algorithm>
#include <pw/impl/move.h>

#include <catch2/catch.hpp>

/*
 * Type Requirements:
 * - No extra
 */
TEMPLATE_LIST_TEST_CASE("Assignment operator", "[vector][operator=]", pw::test::TestTypeListInt)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

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

SCENARIO("Assignment operator", "[vector][operator=]")
{
    using Vector = pw::vector<int, pw::test::allocator_base<int>>;
    REQUIRE(!pw::allocator_traits<Vector::allocator_type>::propagate_on_container_move_assignment::value);
    GIVEN("A vector with allocator move assignment false")
    {
        WHEN("operator=(const_ref) lhs.size() < rhs.size() but less allocated")
        {
            Vector::size_type const capacity = 10;
            Vector                           lhs { 1, 2 };
            Vector                           rhs { 1, 2, 3 };

            lhs = rhs;
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
        WHEN("operator=(const_ref) lhs.size() < rhs.size() but lhs allocated")
        {
            Vector::size_type const capacity = 10;
            Vector                           lhs { 1, 2 };
            Vector                           rhs { 1, 2, 3 };
            lhs.reserve(rhs.size());
            lhs = rhs;
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
        WHEN("operator=(move) lhs.size() < rhs.size() but lhs allocated")
        {
            Vector::size_type const capacity = 10;
            Vector                           lhs { 1, 2 };
            Vector                           rhs { 1, 2, 3 };
            lhs.reserve(rhs.size());
            lhs = pw::move(rhs);
            THEN("they are same")
            {
                REQUIRE(lhs == rhs);
            }
        }
    }
}
