#include <test_input_iterator.h>
#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <pw/algorithm>

/*
 * Type requirements:
 * - No extra
 */
TEMPLATE_LIST_TEST_CASE("Test assign()", "[vector][assign]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector v;
        WHEN("assign(count,value) and lhs is empty")
        {
            value_type value;
            size_t     count = 12;

            pw::test::permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::test::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(count,value) and lhs items")
        {
            value_type value;
            size_t     count = 12;
            v.resize(count - 3);

            pw::test::permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::test::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(begin,end)")
        {
            value_type       value;
            constexpr size_t count         = 3;
            value_type       values[count] = { pw::test::permute_n(value, 4, 1),
                                               pw::test::permute_n(value, 4, 1),
                                               pw::test::permute_n(value, 4, 1) };

            v.assign(&values[0], &values[count]);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are same")
            {
                REQUIRE(pw::equal(&values[0], &values[count], v.begin(), v.end()));
            }
        }
        WHEN("assign(begin,end) with input iterators")
        {
            value_type                                 value;
            constexpr size_t                           count         = 3;
            value_type                                 values[count] = { pw::test::permute_n(value, 4, 1),
                                                                         pw::test::permute_n(value, 4, 1),
                                                                         pw::test::permute_n(value, 4, 1) };
            pw::test::test_input_iterator<value_type*> iter { &values[0] };
            pw::test::test_input_iterator<value_type*> end { &values[count] };
            v.assign(iter, end);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are same")
            {
                Vector expected { values[0], values[1], values[2] };
                REQUIRE(v == expected);
                //REQUIRE(pw::equal(&values[0], &values[count], v.begin(), v.end()));
            }
        }
        WHEN("assign(init_list)")
        {
            value_type value;
            v.assign({ pw::test::permute_n(value, 4, 1),
                       pw::test::permute_n(value, 4, 1),
                       pw::test::permute_n(value, 4, 1) });
            THEN("size() is 3")
            {
                REQUIRE(static_cast<size_t>(3) == v.size());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Test assign() with int", "[vector][assign][int]", pw::test::TestTypeListInt)
{
    using Vector = TestType;
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
