#include <test_copyconstructible.h>
#include <test_defaultcopyconstructible.h>
#include <test_permute.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

/*
 * Type requirements
 * - resize(size_type count): MoveInsertable and DefaultInsertable
 * - resize(size_type count, const value_type& value): CopyInsertable
 */
TEMPLATE_LIST_TEST_CASE("Test resize()",
                        "[vector][resize]",
                        pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("resize() is 0")
        {
            size_t const size = 0;

            v.resize(size);
            THEN("size is is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
        }
        WHEN("resize() increases size")
        {
            size_t const size = 3;

            v.resize(size);
            THEN("size is is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        size_t const count = 5;
        value_type   first_value;
        value_type   last_value;
        value_type   value;
        Vector       values;

        for (size_t i = 0; i < count; ++i)
        {
            pw::test::permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
            last_value = value;
        }
        Vector v(values);

        WHEN("resize() is 0")
        {
            size_t const size = 0;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
        }
        WHEN("resize() increases size")
        {
            size_t const size = count + 3;

            v.resize(size);
            THEN("size is is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
        }
        WHEN("resize() decreases size")
        {
            size_t const capacity = v.capacity();
            size_t const size     = count - 2;

            v.resize(size);
            THEN("size is smaller")
            {
                REQUIRE(size == v.size());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
        }
        WHEN("resize() does not change size")
        {
            size_t const capacity = v.capacity();
            size_t const size     = count;

            v.resize(size);
            THEN("size is unchanged")
            {
                REQUIRE(size == v.size());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
        }
        WHEN("resize() adds elements with value")
        {
            value_type   value;
            size_t const size = count + 3;

            pw::test::permute(value, 3);
            v.resize(size, value);
            THEN("size() is increased")
            {
                REQUIRE(size == v.size());
            }
            THEN("each value is same as permute")
            {
                REQUIRE(value == v[count]);
            }
        }
    }
}
SCENARIO("resize() op counts", "[vector][resize][optracker]")
{
    using Vector     = pw::vector<pw::test::DefaultCopyConstructible>;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init =
        pw::test::DefaultCopyConstructible::getCounter();
    size_t count = 5;

    GIVEN("An empty vector")
    {
        Vector v;

        WHEN("resize() is called")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter();
            v.resize(count);
            counter = pw::test::DefaultCopyConstructible::getCounter() -
                counter;
            THEN("default constructed count times")
            {
                INFO("counter: " << counter);
                REQUIRE(count == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() ==
                        counter.constructorCount());
                REQUIRE(counter.allCount() == count);
            }
        }
    }
    counter = pw::test::DefaultCopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}
