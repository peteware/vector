#include <test_emplacemoveconstructible.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

TEMPLATE_LIST_TEST_CASE("emplace()", "[vector][func][emplace]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    using size_type  = typename Vector::size_type;

    GIVEN("An empty vector")
    {
        Vector     v;
        value_type val;
        pw::test::permute(val, 1);
        WHEN("emplace() at begin()")
        {
            v.emplace(v.begin(), val);
            THEN("size() == 1")
            {
                REQUIRE(v.size() == 1);
            }
            THEN("front() == val")
            {
                REQUIRE(v.front() == val);
            }
        }
    }
    GIVEN("A vector with reserved space")
    {
        Vector          v;
        value_type      val;
        size_type const size = 5;

        pw::test::permute(val, 1);
        v.reserve(size);
        WHEN("emplace() at begin()")
        {
            v.emplace(v.begin(), val);
            THEN("size() == 1")
            {
                REQUIRE(v.size() == 1);
            }
            THEN("front() == val")
            {
                REQUIRE(v.front() == val);
            }
        }
    }
    GIVEN("A vector with elements")
    {
        Vector          v;
        value_type      val;
        size_type const size = 5;

        pw::test::permute(val, 1);
        v.resize(size);
        WHEN("emplace() at begin()")
        {
            v.emplace(v.begin(), val);
            THEN("size() == size+1")
            {
                REQUIRE(v.size() == size + 1);
            }
            THEN("front() == val")
            {
                REQUIRE(v.front() == val);
            }
        }
        WHEN("emplace() at end()")
        {
            v.emplace(v.end(), val);
            THEN("size() == size+1")
            {
                REQUIRE(v.size() == size + 1);
            }
            THEN("back() == val")
            {
                REQUIRE(v.back() == val);
            }
        }
        WHEN("emplace() in middle")
        {
            size_type where = size / 2;
            v.emplace(v.begin() + where, val);
            THEN("size() == size+1")
            {
                REQUIRE(v.size() == size + 1);
            }
            THEN("v[where] == val")
            {
                REQUIRE(v[where] == val);
            }
        }
    }
}
