#include <test_copyconstructible.h>
#include <test_defaultcopyconstructible.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

/*
 * Type requirements
 * - push_back(const T& value): CopyInsertable
 * - push_back(T&& value): MoveInsertable
 */
TEMPLATE_LIST_TEST_CASE("push_back()", "[vector][push_back]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector     v;
        value_type value;

        WHEN("push_back() is called")
        {
            v.push_back(value);
            THEN("capacity() is increased")
            {
                REQUIRE(v.capacity() >= 1);
            }
            THEN("value is there")
            {
                REQUIRE(value == v.front());
                REQUIRE(value == v.back());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        size_t const             count = 5;
        pw::test::Values<Vector> generate(count);
        Vector&                  v = generate.values;

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
    using Vector                   = pw::vector<pw::test::DefaultCopyConstructible>;
    using value_type               = typename Vector::value_type;
    pw::test::OpCounter const init = pw::test::DefaultCopyConstructible::getCounter();
    pw::test::OpCounter       counter;

    GIVEN("An empty vector")
    {
        Vector                             v;
        pw::test::DefaultCopyConstructible copyObject(11);

        WHEN("push_back() is called")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter();
            v.push_back(copyObject);
            THEN("Copy construct called once")
            {
                counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.constructorCount());
                REQUIRE(counter.constructorCount() == counter.allCount());
            }
        }
        WHEN("push_back(move) is called")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter();
            v.push_back(pw::move(copyObject));
            THEN("Copy construct called once")
            {
                counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(counter.getMoveConstructor() == counter.allCount());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector>           generate(5);
        pw::test::DefaultCopyConstructible copyObject(12);
        pw::test::OpCounter                startCount(pw::test::CopyConstructible::getCounter());

        generate.values.shrink_to_fit();
        counter = pw::test::DefaultCopyConstructible::getCounter();
        WHEN("push_back() is called")
        {
            generate.values.push_back(copyObject);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
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
    counter = pw::test::DefaultCopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}
