#include "catch2/catch.hpp"
#include "pw_permute.t.h"
#include <pw/algorithm>
#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

#include <stdexcept>
#include <string>
#include <tuple>

//using TestTypeList = std::tuple<int, std::string, std::vector<int>>;
//using TestTypeList = std::tuple<int, std::string, float>;
using TestTypeList = std::tuple<int>;

TEMPLATE_LIST_TEST_CASE("empty vectors work", "[vector][template]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    Vector v;
    GIVEN("An empty vector of TestType")
    {
        REQUIRE(pw::is_same<TestType*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<TestType, typename Vector::value_type>::value);
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);

        WHEN("get_allocator() const is called")
        {
            typename Vector::allocator_type a = v.get_allocator();
            THEN("it returns same allocator")
            {
                REQUIRE(a == pw::allocator<TestType>());
            }
        }
        WHEN("at() is called")
        {
            THEN("at(0) fails")
            {
                CHECK_THROWS_AS(v.at(0), std::out_of_range);
            }
            THEN("at(10) fails")
            {
                CHECK_THROWS_AS(v.at(10), std::out_of_range);
            }
        }
        WHEN("reserve() is called")
        {
            size_t capacity    = v.capacity();
            size_t newCapacity = capacity + 3;
            v.reserve(newCapacity);
            THEN("capacity increases")
            {
                REQUIRE(newCapacity == v.capacity());
            }
        }
        WHEN("begin() is called")
        {
            typename Vector::iterator iter;
            iter = v.begin();
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
    }
}

TEMPLATE_LIST_TEST_CASE("insert vectors work", "[vector][insert]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    Vector v;
    GIVEN("An empty vector of TestType")
    {
        WHEN("insert() is called")
        {
            typename Vector::iterator iter;
            TestType                  value;
            pw::internal::permute(value, 3);
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
            TestType                  value;
            size_t                    count = 12;

            pw::internal::permute(value, 3);
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
            TestType                  value;
            size_t                    count = 12;

            pw::internal::permute(value, 3);
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
        size_t   count = 5;
        TestType value;
        Vector   values;

        for (size_t i = 0; i < count; ++i)
        {
            pw::internal::permute(value, 10);
            values.push_back(value);
        }
        v = values;
        REQUIRE(pw::equal(values.begin(), values.end(), v.begin(), v.end()));
        TestType newvalue(value);
        pw::internal::permute(newvalue, 10);
        WHEN("insert(3) at begin")
        {
            v.insert(v.begin(), 3, newvalue);
            THEN("size is correct")
            {
                REQUIRE(count + 3 == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(v[0] == newvalue);
                REQUIRE(v[1] == newvalue);
                REQUIRE(v[2] == newvalue);
                REQUIRE(v[3] == values[0]);
                REQUIRE(v[4] == values[1]);
                REQUIRE(v[5] == values[2]);
                REQUIRE(v[6] == values[3]);
                REQUIRE(v[7] == values[4]);
                typename Vector::iterator newpos = v.begin() + 3;
                REQUIRE(pw::equal(values.begin(), values.end(), newpos, newpos + values.size()));
                REQUIRE(pw::equal(values.begin(), values.begin() + 3, newpos, newpos + 3));
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("const vectors work", "[vector][const]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    Vector        v;
    Vector const& c = v;
    GIVEN("An empty const vector of TestType")
    {
        REQUIRE(c.empty());
        REQUIRE(c.size() == 0);
        REQUIRE(c.capacity() == 0);

        WHEN("get_allocator() const is called")
        {
            typename Vector::allocator_type a = v.get_allocator();
            THEN("it returns same allocator")
            {
                REQUIRE(a == pw::allocator<TestType>());
            }
        }
        WHEN("at() const is called")
        {
            THEN("at(0) const fails")
            {
                CHECK_THROWS_AS(c.at(0), std::out_of_range);
            }
            THEN("at(10) const fails")
            {
                CHECK_THROWS_AS(c.at(10), std::out_of_range);
            }
        }
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

TEMPLATE_LIST_TEST_CASE("vector constructors", "[vector][constructor]", TestTypeList)
{
    using Vector = pw::vector<TestType>;

    GIVEN("A vector constructed with a count")
    {
        size_t const count = 3;
        Vector       v(count);

        WHEN("empty() is called")
        {
            bool e = v.empty();
            THEN("it is not empty")
            {
                REQUIRE(!e);
            }
        }
        WHEN("size() is called")
        {
            size_t s = v.size();
            THEN("it is the same as count")
            {
                REQUIRE(count == s);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("capacity is same")
            {
                REQUIRE(count == c);
            }
        }
        WHEN("copy constructor is called")
        {
            Vector c(v);
            THEN("two vectors are same")
            {
                REQUIRE(v.size() == c.size());
                REQUIRE(v[0] == c[0]);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("vectors work", "[vector][empty]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    Vector v;
    v.push_back(TestType());
    GIVEN("A vector of TestType with 1 item")
    {
        WHEN("empty() is called")
        {
            bool e = v.empty();
            THEN("it is not empty")
            {
                REQUIRE(!e);
            }
        }
        WHEN("size() is called")
        {
            size_t s = v.size();
            THEN("size is 1")
            {
                REQUIRE(s == 1);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("it is at least 1)")
            {
                REQUIRE(c >= 1);
            }
        }
        WHEN("at(0) is called")
        {
            TestType& r = v.at(0);
            THEN("it works")
            {
                REQUIRE(r == TestType());
            }
        }
        WHEN("at(1) is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(v.at(1), std::out_of_range);
            }
        }
        WHEN("front() is called")
        {
            TestType& r = v.front();
            THEN("it works")
            {
                REQUIRE(r == TestType());
            }
        }
        WHEN("back() is called")
        {
            TestType& r = v.back();
            THEN("it works")
            {
                REQUIRE(r == TestType());
            }
        }
        WHEN("data() is called")
        {
            TestType* p = v.data();
            THEN("it works")
            {
                REQUIRE(*p == TestType());
            }
        }
        WHEN("reserve() is called")
        {
            size_t capacity    = v.capacity();
            size_t newCapacity = capacity + 3;
            v.reserve(newCapacity);
            THEN("capacity increases")
            {
                REQUIRE(newCapacity == v.capacity());
            }
        }
        WHEN("reserve() is called with current capacity")
        {
            size_t capacity = v.capacity();
            v.reserve(capacity);
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
        }
        WHEN("push_back() const_ref is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                TestType t;
                v.push_back(t);
            }
            REQUIRE(capacity < v.capacity());
        }
        WHEN("push_back() move is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                v.push_back(TestType());
            }
            REQUIRE(capacity < v.capacity());
        }
    }
}

TEMPLATE_LIST_TEST_CASE("const vectors access", "[vector][single]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    Vector        v;
    Vector const& c = v;

    v.push_back(TestType());

    GIVEN("A const vector of TestType with 1 item")
    {
        WHEN("at(0) const is called")
        {
            TestType const& r = c.at(0);
            THEN("it works")
            {
                REQUIRE(r == TestType());
            }
        }
        WHEN("at(1) const is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(c.at(1), std::out_of_range);
            }
        }
        WHEN("front() const is called")
        {
            TestType const& r = v.front();
            THEN("it works")
            {
                REQUIRE(r == TestType());
            }
        }
        WHEN("back() const is called")
        {
            TestType const& r = v.back();
            THEN("it works")
            {
                REQUIRE(r == TestType());
            }
        }
        WHEN("data() const is called")
        {
            TestType const* p = v.data();
            THEN("it works")
            {
                REQUIRE(*p == TestType());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("vector resize", "[vector][resize]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    GIVEN("A vector of TestType with 3 elements")
    {
        size_t const initsize = 3;
        Vector       v(initsize);

        REQUIRE(initsize == v.size());
        REQUIRE(initsize == v.capacity());
        WHEN("resize() increases size")
        {
            size_t const size = initsize + 3;

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
            size_t const size     = initsize - 2;

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
            size_t const size     = initsize;

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
            TestType     value;
            size_t const size = initsize + 3;

            pw::internal::permute(value, 3);
            v.resize(size, value);
            THEN("size() is increased")
            {
                REQUIRE(size == v.size());
            }
            THEN("each value is same as permute")
            {
                REQUIRE(value == v[initsize]);
            }
        }
    }
}
