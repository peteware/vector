#include "catch2/catch.hpp"
#include "permute.h"
#include "same.t.h"
#include <pw/algorithm>
#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using TestTypeList = std::tuple<pw::vector<int>, pw::vector<std::string>, std::vector<int>>;
//using TestTypeList = std::tuple<pw::vector<int>, std::vector<int>, std::vector<std::string>>;

TEMPLATE_LIST_TEST_CASE("Test constructors", "[vector][constructor]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
}

TEMPLATE_LIST_TEST_CASE("Test insert", "[vector][insert]", TestTypeList)
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
            permute(value, 3);
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

            permute(value, 3);
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

            permute(value, 3);
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
}

TEMPLATE_LIST_TEST_CASE("const methods on empty vector", "[vector][empty]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        REQUIRE(pw::is_same<value_type*, typename Vector::pointer>::value);
        REQUIRE(pw::is_same<value_type, typename Vector::value_type>::value);
        // WHEN("get_allocator() const is called")
        // {
        //     typename Vector::allocator_type a = v.get_allocator();
        //     THEN("it returns same allocator")
        //     {
        //         REQUIRE(a == pw::allocator<value_type>());
        //     }
        // }
        WHEN("operator=(const_ref) both empty")
        {
            Vector op2;
            op2 = v;
            THEN("equal")
            {
                REQUIRE(pw::equal(v.begin(), v.end(), op2.begin(), op2.end()));
            }
        }
        WHEN("operator=(const_ref) lhs has elements")
        {
            Vector op2(5);
            op2 = v;
            THEN("equal")
            {
                REQUIRE(pw::equal(v.begin(), v.end(), op2.begin(), op2.end()));
            }
        }
        WHEN("operator=(const_ref) rhs has elements")
        {
            Vector op2(5);
            v = op2;
            THEN("equal")
            {
                REQUIRE(pw::equal(v.begin(), v.end(), op2.begin(), op2.end()));
            }
        }
        WHEN("operator=(move) both empty")
        {
            Vector op2;
            op2 = pw::move(v);
            THEN("size() is same")
            {
                REQUIRE(v.size() == op2.size());
            }
        }
        WHEN("operator=(move) lhs has elements")
        {
            Vector op2(5);
            op2 = pw::move(v);
            THEN("size() is same")
            {
                REQUIRE(v.size() == op2.size());
            }
        }
        WHEN("operator=(move) rhs has elements")
        {
            Vector op2(5);
            v = pw::move(op2);
            THEN("size() is same")
            {
                REQUIRE(v.size() == 5);
            }
        }
        WHEN("operator=(initializer_list) lhs is empty")
        {
            v = { value_type(), value_type(), value_type() };

            THEN("size() is 3")
            {
                REQUIRE(v.size() == 3);
            }
        }
        WHEN("operator=(initializer_list) lhs has elements")
        {
            v.resize(10);
            v = { value_type(), value_type(), value_type() };

            THEN("size() is 3")
            {
                REQUIRE(v.size() == 3);
            }
        }
        WHEN("assign(count,value) and lhs is empty")
        {
            value_type value;
            size_t     count = 12;

            permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::internal::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(count,value) and lhs items")
        {
            value_type value;
            size_t     count = 12;
            v.resize(count - 3);

            permute(value, 3);
            v.assign(count, value);
            THEN("size() is count")
            {
                REQUIRE(count == v.size());
            }
            THEN("all elements are value")
            {
                REQUIRE(pw::internal::same(v.begin(), v.end(), value));
            }
        }
        WHEN("assign(begin,end)")
        {
            value_type   value;
            size_t const count         = 3;
            value_type   values[count] = { permute_n(value, 4, 1),
                                         permute_n(value, 4, 1),
                                         permute_n(value, 4, 1) };

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
        WHEN("assign(init_list)")
        {
            value_type value;
            v.assign({ permute_n(value, 4, 1), permute_n(value, 4, 1), permute_n(value, 4, 1) });
            THEN("size() is 3")
            {
                REQUIRE(static_cast<size_t>(3) == v.size());
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
        GIVEN("and it is const")
        {
            Vector const& c = v;
            REQUIRE(c.empty());
            REQUIRE(c.size() == 0);
            REQUIRE(c.capacity() == 0);

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
    GIVEN("A vector with 5 elements")
    {
        size_t const count = 5;
        value_type   first_value;
        value_type   last_value;
        value_type   value;
        Vector       values;

        for (size_t i = 0; i < count; ++i)
        {
            permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
            last_value = value;
        }
        Vector v(values);
        REQUIRE(pw::equal(values.begin(), values.end(), v.begin(), v.end()));
        permute(value, 10);

        size_t                    added;
        size_t                    offset;
        typename Vector::iterator where;
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
        WHEN("insert(3) at begin")
        {
            added  = 3;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(3) at end")
        {
            added  = 3;
            offset = v.size();
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at begin")
        {
            added  = 10;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(10) at near end")
        {
            added  = 10;
            offset = count - 1;
            where  = v.begin() + offset;
            where  = v.insert(where, added, value);
            THEN("size is correct")
            {
                REQUIRE(count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::internal::same(where, where + added, value));
                REQUIRE(pw::equal(values.begin(), values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(values.begin() + offset, values.end(), where + added, v.end()));
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

            permute(value, 3);
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
            THEN("size is count")
            {
                REQUIRE(s == count);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("it is at least 1)")
            {
                REQUIRE(c >= count);
            }
        }
        WHEN("at(0) is called")
        {
            value_type& r = v.at(0);
            THEN("it works")
            {
                REQUIRE(first_value == r);
            }
        }
        WHEN("at(count) is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(v.at(count), std::out_of_range);
            }
        }
        WHEN("front() is called")
        {
            value_type& r = v.front();
            THEN("it works")
            {
                REQUIRE(first_value == r);
            }
        }
        WHEN("back() is called")
        {
            value_type& r = v.back();
            THEN("it works")
            {
                REQUIRE(last_value == r);
            }
        }
        WHEN("data() is called")
        {
            value_type* p = v.data();
            THEN("it works")
            {
                REQUIRE(first_value == *p);
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
        GIVEN("And extra capacity")
        {
            size_t const total = count + 10;
            v.reserve(total);
            REQUIRE(total == v.capacity());
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
    GIVEN("A const vector of value_type with 1 item")
    {
        Vector        v;
        Vector const& c = v;

        v.push_back(value_type());

        WHEN("at(0) const is called")
        {
            value_type const& r = c.at(0);
            THEN("it works")
            {
                REQUIRE(r == value_type());
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
            value_type const& r = v.front();
            THEN("it works")
            {
                REQUIRE(r == value_type());
            }
        }
        WHEN("back() const is called")
        {
            value_type const& r = v.back();
            THEN("it works")
            {
                REQUIRE(r == value_type());
            }
        }
        WHEN("data() const is called")
        {
            value_type const* p = v.data();
            THEN("it works")
            {
                REQUIRE(*p == value_type());
            }
        }
    }
}
