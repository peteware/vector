#include <catch2/catch.hpp>
#include <pw/algorithm>
#include <test_optracker.h>
#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

TEMPLATE_LIST_TEST_CASE("push_back()", "[vector][push_back]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
    }
    GIVEN("A vector with 5 elements")
    {
        size_t const   count = 5;
        Values<Vector> generate(count);
        Vector&        v = generate.values;

        WHEN("push_back() const_ref is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                value_type t;
                v.push_back(t);
            }
            REQUIRE(capacity < v.capacity());
        }
        WHEN("push_back() move is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                v.push_back(value_type());
            }
            REQUIRE(capacity < v.capacity());
        }
    }
}

SCENARIO("push_back() op counts", "[vector][push_back][optracker]")
{
    using Vector     = pw::vector<pw::test::CopyConstructible>;
    using value_type = typename Vector::value_type;
    pw::test::OpCounter       counter;
    pw::test::OpCounter const init = pw::test::CopyConstructible::getCounter();

    GIVEN("An empty vector")
    {
        Vector                      v;
        pw::test::CopyConstructible copyObject;

        WHEN("push_back() is called")
        {
            counter = pw::test::CopyConstructible::getCounter();
            v.push_back(copyObject);
            THEN("Copy construct called once")
            {
                counter = pw::test::CopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.constructorCount());
                REQUIRE(counter.constructorCount() == counter.allCount());
            }
        }
        WHEN("push_back(move) is called")
        {
            counter = pw::test::CopyConstructible::getCounter();
            v.push_back(pw::move(copyObject));
            THEN("Copy construct called once")
            {
                counter = pw::test::CopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(counter.getMoveConstructor() == counter.allCount());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        Values<Vector>              generate(5);
        pw::test::CopyConstructible copyObject;
        pw::test::OpCounter         startCount(pw::test::CopyConstructible::getCounter());

        generate.values.shrink_to_fit();
        counter = pw::test::CopyConstructible::getCounter();
        WHEN("push_back() is called")
        {
            generate.values.push_back(copyObject);
            counter = pw::test::CopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items")
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
