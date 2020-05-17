#include <test_testtype.h>

#include <catch2/catch.hpp>

TEMPLATE_LIST_TEST_CASE("pop_back()", "[vector][pop_back]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("A vector with 5 elements")
    {
        size_t const             count = 5;
        pw::test::Values<Vector> generate(count);
        Vector&                  v        = generate.values;
        size_t const             capacity = v.capacity();

        WHEN("pop_back() is called")
        {
            v.pop_back();
            THEN("size is 1 smaller")
            {
                REQUIRE(generate.count == v.size() + 1);
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
        }
    }
    GIVEN("A vector with 1 element")
    {
        size_t const             count = 1;
        pw::test::Values<Vector> generate(count);
        Vector&                  v        = generate.values;
        size_t const             capacity = v.capacity();

        WHEN("pop_back() is called")
        {
            v.pop_back();
            THEN("size is 1 smaller")
            {
                REQUIRE(generate.count == v.size() + 1);
            }
            THEN("it is empty")
            {
                REQUIRE(v.empty());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
        }
    }
}
