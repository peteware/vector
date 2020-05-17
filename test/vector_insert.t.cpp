#include <catch2/catch.hpp>
#include <pw/algorithm>
#include <test_optracker.h>
#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("Test insert", "[vector][insert]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("insert() is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 3);
            iter = v.insert(v.begin(), value);
            THEN("size() is increased")
            {
                REQUIRE(1 == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
            }
            THEN("at(0) returns same value")
            {
                REQUIRE(value == v.at(0));
            }
        }
        WHEN("insert(count) at begin()")
        {
            typename Vector::iterator iter;
            value_type                value;
            size_t                    count = 12;

            pw::test::permute(value, 3);
            iter = v.insert(v.begin(), count, value);
            THEN("there are count values")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(count == v.size());
                REQUIRE(value == v[0]);
                REQUIRE(value == v[count - 1]);
            }
        }
        WHEN("insert(12) at end()")
        {
            typename Vector::iterator iter;
            value_type                value;
            size_t                    count = 12;

            pw::test::permute(value, 3);
            iter = v.insert(v.end(), count, value);
            THEN("there are count values")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(count == v.size());
                REQUIRE(value == v[0]);
                REQUIRE(value == v[count - 1]);
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector>  generate(5);
        size_t                    added;
        size_t                    offset;
        value_type                value;
        typename Vector::iterator where;

        Vector v(generate.values);
        WHEN("insert(3) at begin")
        {
            added  = 3;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(3) at end")
        {
            added  = 3;
            offset = v.size();
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at begin")
        {
            added  = 10;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at near end")
        {
            added  = 10;
            offset = generate.count - 1;
            where  = v.begin() + offset;
            where  = v.insert(where, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
    }
}

SCENARIO("insert() op counts", "[vector][insert][optracker]")
{
    using Vector     = pw::vector<pw::test::CopyConstructible>;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init  = pw::test::CopyConstructible::getCounter();
    size_t                    count = 5;

    GIVEN("An empty vector")
    {
        Vector                      v;
        pw::test::CopyConstructible copyObject;

        WHEN("insert(count) at begin")
        {
            pw::test::CopyConstructible copyObject;
            counter = pw::test::CopyConstructible::getCounter();
            v.insert(v.begin(), count, copyObject);
            THEN("Copy constructor called count times")
            {
                counter = pw::test::CopyConstructible::getCounter() - counter;
                REQUIRE(count == counter.constructorCount());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector>    generate(5);
        pw::test::CopyConstructible copyObject;

        generate.values.shrink_to_fit();
        counter = pw::test::CopyConstructible::getCounter();
        WHEN("insert() at begin")
        {
            generate.values.insert(generate.values.begin(), copyObject);
            counter = pw::test::CopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items for more space")
            {
                REQUIRE(generate.count == counter.getMoveConstructor());
            }
            THEN("Assigned item")
            {
                REQUIRE(1 == counter.getCopyConstructor());
            }
        }
        WHEN("insert() at end")
        {
            generate.values.insert(generate.values.end(), copyObject);
            counter = pw::test::CopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items for more space")
            {
                REQUIRE(generate.count == counter.getMoveConstructor());
            }
            THEN("Copy constructed new item")
            {
                REQUIRE(1 == counter.getCopyConstructor());
            }
        }
        WHEN("insert() in middle")
        {
            generate.values.insert(generate.values.begin() + generate.count - 2, copyObject);
            counter = pw::test::CopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items for more space")
            {
                REQUIRE(generate.count == counter.getMoveConstructor());
            }
            THEN("Copy constructed new item")
            {
                REQUIRE(1 == counter.getCopyConstructor());
            }
        }
    }
    counter = pw::test::CopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}
