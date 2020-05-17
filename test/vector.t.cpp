#include <catch2/catch.hpp>
#include <pw/algorithm>
#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

#include <stdexcept>
//using TestTypeList = std::tuple<pw::vector<int>, std::vector<int>, std::vector<std::string>>;

TEMPLATE_LIST_TEST_CASE("Test constructors", "[vector][constructor]", TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
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
        Values<Vector> generate(5);
        Vector         v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

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
                REQUIRE(generate.count == s);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("capacity is same")
            {
                REQUIRE(generate.count == c);
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
        WHEN("empty() is called")
        {
            bool e = v.empty();
            THEN("it is not empty")
            {
                REQUIRE(!e);
            }
        }
        WHEN("capacity() is called")
        {
            size_t c = v.capacity();
            THEN("it is at least 1)")
            {
                REQUIRE(c >= generate.count);
            }
        }
        WHEN("front() is called")
        {
            value_type& r = v.front();
            THEN("it works")
            {
                REQUIRE(generate.first_value == r);
            }
        }
        WHEN("back() is called")
        {
            value_type& r = v.back();
            THEN("it works")
            {
                REQUIRE(generate.last_value == r);
            }
        }
        WHEN("data() is called")
        {
            value_type* p = v.data();
            THEN("it works")
            {
                REQUIRE(generate.first_value == *p);
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
        GIVEN("And extra capacity")
        {
            size_t const total = generate.count + 10;
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
        Values<Vector> generate(1);
        Vector const&  c = generate.values;

        WHEN("front() const is called")
        {
            value_type const& r = c.front();
            THEN("it works")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("back() const is called")
        {
            value_type const& r = c.back();
            THEN("it works")
            {
                REQUIRE(r == generate.last_value);
            }
        }
        WHEN("data() const is called")
        {
            value_type const* p = c.data();
            THEN("it works")
            {
                REQUIRE(*p == generate.first_value);
            }
        }
    }
}
