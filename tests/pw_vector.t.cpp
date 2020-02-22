#include "catch2/catch.hpp"
#include "pw_permute.t.h"
#include "pw_same.t.h"
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

TEMPLATE_LIST_TEST_CASE("const methods on empty vector", "[vector][empty]", TestTypeList)
{
    using Vector = pw::vector<TestType>;
    Vector v;
    GIVEN("An empty vector of TestType")
    {
        REQUIRE(pw::is_same<TestType*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<TestType, typename Vector::value_type>::value);
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
        WHEN("data() is called")
        {
            THEN("it returns NULL")
            {
                REQUIRE(!v.data());
            }
        }
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
        WHEN("empty() is called")
        {
            THEN("it is true")
            {
                REQUIRE(v.empty());
            }
        }
        WHEN("size() is called")
        {
            THEN("it is 0")
            {
                REQUIRE(0 == v.size());
            }
        }
        WHEN("max_size() is called")
        {
            THEN("it is bigger then 0")
            {
                REQUIRE(10000 < v.max_size());
            }
        }
        WHEN("capacity() is called")
        {
            THEN("it is 0")
            {
                REQUIRE(0 == v.capacity());
            }
        }
        WHEN("shrink_to_fit() is called")
        {
            v.shrink_to_fit();
            THEN("nothing goes wrong")
            {
                REQUIRE(v.empty());
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
        WHEN("clear() is called")
        {
            v.clear();
            THEN("nothing goes wrong")
            {
                REQUIRE(v.empty());
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
        size_t const count = 5;
        TestType     value;
        Vector       values;

        for (size_t i = 0; i < count; ++i)
        {
            pw::internal::permute(value, 10);
            values.push_back(value);
        }
        v = values;
        REQUIRE(pw::equal(values.begin(), values.end(), v.begin(), v.end()));
        TestType newvalue(value);
        pw::internal::permute(newvalue, 10);

        size_t                    added;
        size_t                    offset;
        typename Vector::iterator where;
        WHEN("insert(3) at begin")
        {
            added  = 3;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, newvalue);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, newvalue));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(3) at end")
        {
            added  = 3;
            offset = v.size();
            where  = v.insert(v.begin() + offset, added, newvalue);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, newvalue));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at begin")
        {
            added  = 10;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, newvalue);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, newvalue));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at near end")
        {
            added  = 10;
            offset = count - 1;
            where  = v.begin() + offset;
            where  = v.insert(where, added, newvalue);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, newvalue));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
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

    size_t const count = 3;
    Vector       v(count);

    GIVEN("A vector constructed with a count")
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
                REQUIRE(pw::equal(v.begin(), v.end(), c.begin(), c.end()));
            }
        }
        WHEN("move constructor is called")
        {
            Vector c(v);
            Vector d(pw::move(v));
            THEN("two vectors are same")
            {
                REQUIRE(pw::equal(c.begin(), c.end(), d.begin(), d.end()));
            }
        }
        WHEN("move constructor is called")
        {
            Vector c(v);
            Vector d(pw::move(v));
            THEN("two vectors are same")
            {
                REQUIRE(pw::equal(c.begin(), c.end(), d.begin(), d.end()));
            }
        }
        WHEN("move constructor is called")
        {
            Vector c(v);
            Vector d(pw::move(v));
            THEN("two vectors are same")
            {
                REQUIRE(pw::equal(c.begin(), c.end(), d.begin(), d.end()));
            }
        }
    }
    GIVEN("A vector with elements and extra space")
    {
        v.reserve(10);
        REQUIRE(10 == v.capacity());
        WHEN("shrink_to_fit() is called")
        {
            v.shrink_to_fit();
            THEN("size() and capacity() are same")
            {
                REQUIRE(v.size() == v.capacity());
            }
        }

        WHEN("clear() is called then shrink_to_fit()")
        {
            v.clear();
            v.shrink_to_fit();
            THEN("size() is 0")
            {
                REQUIRE(0 == v.size());
            }
            THEN("capacity() is 0")
            {
                REQUIRE(0 == v.capacity());
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
