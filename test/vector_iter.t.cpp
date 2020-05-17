#include <test_testtype.h>

#include <catch2/catch.hpp>

TEMPLATE_LIST_TEST_CASE("iterator methods on empty vector", "[vector][iter][empty]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        REQUIRE(pw::is_same<value_type*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<value_type, typename Vector::value_type>::value);
        WHEN("begin() is called")
        {
            typename Vector::iterator iter;
            iter = v.begin();
            THEN("cbegin() is same")
            {
                REQUIRE(iter == v.cbegin());
            }
            THEN("cend() is same")
            {
                REQUIRE(iter == v.begin());
            }
            THEN("end() is same")
            {
                REQUIRE(iter == v.end());
            }
            THEN("cend() is same as end()")
            {
                REQUIRE(iter == v.cend());
            }
        }
        GIVEN("and it is const")
        {
            Vector const& c = v;
            REQUIRE(c.empty());
            REQUIRE(c.size() == 0);
            REQUIRE(c.capacity() == 0);

            WHEN("begin() is called")
            {
                typename Vector::const_iterator iter;
                iter = v.begin();
                THEN("cend() is same")
                {
                    REQUIRE(iter == v.cend());
                }
                THEN("cbegin() is same")
                {
                    REQUIRE(iter == v.cbegin());
                }
                THEN("end() is same")
                {
                    REQUIRE(iter == v.end());
                }
                THEN("cend() is same as end()")
                {
                    REQUIRE(iter == v.cend());
                }
            }
        }
    }
}
