#include <pw/vector>
#include <test_defaultcopyconstructible.h>
#include <test_testtype.h>
#include <test_values.h>

#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

using TestTypeList = std::tuple<pw::vector<pw::test::DefaultCopyConstructible>,
                                std::vector<pw::test::DefaultCopyConstructible>>;
/*
 * Type requirements:
 * - No extra
 */
TEMPLATE_LIST_TEST_CASE("count constructors in vector", "[vector][constructor]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init(pw::test::DefaultCopyConstructible::getCounter());

    GIVEN("An empty vector")
    {
        Vector       v;
        size_t const count = 5;

        WHEN("Nothing was called")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter() - init;
            THEN("Nothing was constructed") { REQUIRE(counter.zero()); }
        }
        WHEN("reserve() is increased")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter();
            v.reserve(5);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
            THEN("copy constructor not called") { REQUIRE(counter.zero()); }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

        WHEN("copy constructor is called")
        {
            Vector c(v);
            THEN("two vectors are same") { REQUIRE(pw::equal(v.begin(), v.end(), c.begin(), c.end())); }
        }
        WHEN("move constructor is called")
        {
            Vector c(v);
            Vector d(pw::move(v));
            THEN("two vectors are same") { REQUIRE(pw::equal(c.begin(), c.end(), d.begin(), d.end())); }
        }
        WHEN("move constructor is called")
        {
            Vector c(v);
            Vector d(pw::move(v));
            THEN("two vectors are same")
            {
                REQUIRE(pw::equal(c.begin(), c.end(), d.begin(), d.end()));
                REQUIRE(c == d);
            }
        }
        WHEN("move constructor is called")
        {
            Vector c(v);
            Vector d(pw::move(v));
            THEN("two vectors are same")
            {
                REQUIRE(pw::equal(c.begin(), c.end(), d.begin(), d.end()));
                REQUIRE(c == d);
            }
        }
    }
    GIVEN("and add count objects")
    {
        size_t const                       count = 5;
        pw::test::DefaultCopyConstructible copyObject(-1);
        pw::test::OpCounter                startCount(pw::test::DefaultCopyConstructible::getCounter());
        Vector                             v(count);

        counter = pw::test::DefaultCopyConstructible::getCounter();

        REQUIRE(v.size() == v.capacity());
        WHEN("getCounter()")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter() - startCount;
            THEN("count items default constructed")
            {
                REQUIRE(count == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() == counter.constructorCount());
            }
        }
    }
    counter = pw::test::DefaultCopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

TEMPLATE_LIST_TEST_CASE("init-list", "[vector][constructor][init-list]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter               init(pw::test::DefaultCopyConstructible::getCounter());
    pw::test::OpCounter               counter;
    std::initializer_list<value_type> initlist = { 1, 2, 5 };

    counter = pw::test::DefaultCopyConstructible::getCounter() - init;
    INFO("counter: " << counter);
    REQUIRE(3 == counter.getOtherConstructor());
    REQUIRE(3 == counter.constructorCount());
    GIVEN("A vector from an init list")
    {
        init = pw::test::DefaultCopyConstructible::getCounter();
        Vector v { initlist };
        WHEN("nothing is changed")
        {
            THEN("size() is same") { REQUIRE(3 == v.size()); }
            THEN("capacity() is same") { REQUIRE(v.size() == v.capacity()); }
            THEN("constructors called")
            {
                counter = pw::test::DefaultCopyConstructible::getCounter() - init;
                REQUIRE(3 == counter.constructorCount());
                REQUIRE(3 == counter.getCopyConstructor());
            }
        }
        WHEN("assign vector")
        {
            Vector v2;
            v2.push_back(pw::test::DefaultCopyConstructible(1));
            v2 = v;
            THEN("They are the same") { REQUIRE(v == v2); }
        }
        WHEN("copy construct")
        {
            Vector v2(v);
            THEN("They are the same") { REQUIRE(v == v2); }
        }
    }
}
