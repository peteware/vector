#include <pw/vector>

#include <test_testtype.h>

#include <catch2/catch.hpp>

TEMPLATE_LIST_TEST_CASE("Assignment operator", "[vector][operator=]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("operator=(const_ref) both empty")
        {
            Vector op2;
            op2 = v;
            THEN("equal")
            {
                REQUIRE(pw::equal(v.begin(), v.end(), op2.begin(), op2.end()));
            }
        }
        WHEN("operator=(const_ref) lhs has elements")
        {
            Vector op2(5);
            op2 = v;
            THEN("equal")
            {
                REQUIRE(pw::equal(v.begin(), v.end(), op2.begin(), op2.end()));
            }
        }
        WHEN("operator=(const_ref) rhs has elements")
        {
            Vector op2(5);
            v = op2;
            THEN("equal")
            {
                REQUIRE(pw::equal(v.begin(), v.end(), op2.begin(), op2.end()));
            }
        }
        WHEN("operator=(move) both empty")
        {
            Vector op2;
            op2 = pw::move(v);
            THEN("size() is same")
            {
                REQUIRE(v.size() == op2.size());
            }
        }
        WHEN("operator=(move) lhs has elements")
        {
            Vector op2(5);
            op2 = pw::move(v);
            THEN("size() is same")
            {
                REQUIRE(v.size() == op2.size());
            }
        }
        WHEN("operator=(move) rhs has elements")
        {
            Vector op2(5);
            v = pw::move(op2);
            THEN("size() is same")
            {
                REQUIRE(v.size() == 5);
            }
        }
        WHEN("operator=(initializer_list) lhs is empty")
        {
            v = { value_type(), value_type(), value_type() };

            THEN("size() is 3")
            {
                REQUIRE(v.size() == 3);
            }
        }
        WHEN("operator=(initializer_list) lhs has elements")
        {
            v.resize(10);
            v = { value_type(), value_type(), value_type() };

            THEN("size() is 3")
            {
                REQUIRE(v.size() == 3);
            }
        }
    }
}
