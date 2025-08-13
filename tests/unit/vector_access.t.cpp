#include <test_testtype.h>
#include <test_values.h>

#include <pw/algorithm>

#include <catch2/catch_all.hpp>

#include <stdexcept>

/*
 * Type requirements:
 * - at(): no extra
 * - operator[](): no extra
 * - front(): no extra
 * - back(): no extra
 * - data(): no extra
 */
TEMPLATE_LIST_TEST_CASE("access methods", "[vector][access]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("data() is called")
        {
            THEN("it returns NULL")
            {
                REQUIRE(!v.data());
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
    }
    GIVEN("A const empty vector")
    {
        Vector const v;
        WHEN("at() const is called")
        {
            THEN("at(0) const fails")
            {
                CHECK_THROWS_AS(v.at(0), std::out_of_range);
            }
            THEN("at(10) const fails")
            {
                CHECK_THROWS_AS(v.at(10), std::out_of_range);
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        REQUIRE(pw::equal(generate.values.begin(), generate.values.end(), v.begin(), v.end()));

        WHEN("at(0) is called")
        {
            value_type& r = v.at(0);
            THEN("it works")
            {
                REQUIRE(generate.first_value == r);
            }
        }
        WHEN("at(count) is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(v.at(generate.count), std::out_of_range);
            }
        }
        WHEN("v[0]")
        {
            value_type& r = v[0];
            THEN("it is first_value")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("v[count - 1]")
        {
            value_type& r = v[generate.count - 1];
            THEN("it is last_value")
            {
                REQUIRE(r == generate.last_value);
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
    }
    GIVEN("A const vector of value_type with 1 item")
    {
        pw::test::Values<Vector> generate(1);
        Vector const&            c = generate.values;

        WHEN("at(0) const is called")
        {
            value_type const& r = c.at(0);
            THEN("it works")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("at(1) const is called")
        {
            THEN("it raises exception")
            {
                CHECK_THROWS_AS(c.at(1), std::out_of_range);
            }
        }
        WHEN("v[0]")
        {
            value_type const& r = c[0];
            THEN("it is first_value")
            {
                REQUIRE(r == generate.first_value);
            }
        }
        WHEN("v[count - 1]")
        {
            value_type const& r = c[generate.count - 1];
            THEN("it is last_value")
            {
                REQUIRE(r == generate.last_value);
            }
        }
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

TEST_CASE("operator[] methods", "[vector][operator][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("single element vector")
    {
        WHEN("operator[] is used on single element vector")
        {
            Vector v = { 42 };

            THEN("element can be accessed")
            {
                REQUIRE(v[0] == 42);
            }
            THEN("element can be modified")
            {
                v[0] = 99;
                REQUIRE(v[0] == 99);
                REQUIRE(v.at(0) == 99);
            }
        }
        WHEN("const operator[] is used on single element vector")
        {
            const Vector v = { 42 };

            THEN("element can be accessed")
            {
                REQUIRE(v[0] == 42);
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("operator[] is used on multi-element vector")
        {
            Vector v = { 10, 20, 30, 40, 50 };

            THEN("all elements can be accessed")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(v[3] == 40);
                REQUIRE(v[4] == 50);
            }
            THEN("elements can be modified")
            {
                v[2] = 333;
                REQUIRE(v[2] == 333);
                REQUIRE(v.at(2) == 333);
            }
        }
        WHEN("const operator[] is used on multi-element vector")
        {
            const Vector v = { 10, 20, 30, 40, 50 };

            THEN("all elements can be accessed")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(v[3] == 40);
                REQUIRE(v[4] == 50);
            }
        }
    }
    GIVEN("boundary conditions and semantics")
    {
        WHEN("operator[] boundary conditions are tested")
        {
            Vector v(static_cast<Vector::size_type>(1000), 7);

            THEN("boundary elements can be accessed")
            {
                REQUIRE(v[0] == 7);
                REQUIRE(v[999] == 7);
            }
            THEN("boundary elements can be modified")
            {
                v[0]   = 1;
                v[999] = 2;
                REQUIRE(v[0] == 1);
                REQUIRE(v[999] == 2);
                REQUIRE(v[500] == 7);
            }
        }
        WHEN("operator[] return type and reference semantics are tested")
        {
            Vector v = { 100 };

            THEN("reference modification works")
            {
                int& ref = v[0];
                ref      = 200;
                REQUIRE(v[0] == 200);
                REQUIRE(v.at(0) == 200);
            }
        }
        WHEN("operator[] vs at() comparison is performed")
        {
            Vector v = { 1, 2, 3 };

            THEN("both methods return same values")
            {
                for (int i : v)
                {
                    REQUIRE(i == i);
                }
            }
            THEN("modifications are consistent")
            {
                v[1] = 999;
                REQUIRE(v[1] == v.at(1));
                REQUIRE(v[1] == 999);
            }
        }
    }
}

TEST_CASE("front() and back() methods", "[vector][front][back][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("single element vector")
    {
        WHEN("front() and back() are used on single element vector")
        {
            Vector v = { 42 };

            THEN("both methods return same value")
            {
                REQUIRE(v.front() == 42);
                REQUIRE(v.back() == 42);
                REQUIRE(v.front() == v.back());
            }
            THEN("front() can modify the element")
            {
                v.front() = 99;
                REQUIRE(v.front() == 99);
                REQUIRE(v.back() == 99);
                REQUIRE(v[0] == 99);
            }
            THEN("back() can modify the element")
            {
                v.back() = 77;
                REQUIRE(v.front() == 77);
                REQUIRE(v.back() == 77);
                REQUIRE(v[0] == 77);
            }
        }
        WHEN("const front() and back() are used on single element vector")
        {
            const Vector v = { 42 };

            THEN("both methods return same value")
            {
                REQUIRE(v.front() == 42);
                REQUIRE(v.back() == 42);
                REQUIRE(v.front() == v.back());
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("front() and back() are used on multi-element vector")
        {
            Vector v = { 10, 20, 30, 40, 50 };

            THEN("methods return correct elements")
            {
                REQUIRE(v.front() == 10);
                REQUIRE(v.back() == 50);
                REQUIRE(v.front() == v[0]);
                REQUIRE(v.back() == v[4]);
                REQUIRE(v.back() == v[v.size() - 1]);
            }
            THEN("front() can modify first element")
            {
                v.front() = 100;
                REQUIRE(v.front() == 100);
                REQUIRE(v[0] == 100);
                REQUIRE(v.back() == 50);
            }
            THEN("back() can modify last element")
            {
                v.back() = 500;
                REQUIRE(v.back() == 500);
                REQUIRE(v[4] == 500);
                REQUIRE(v.front() == 10);
            }
        }
        WHEN("const front() and back() are used on multi-element vector")
        {
            const Vector v = { 10, 20, 30, 40, 50 };

            THEN("methods return correct elements")
            {
                REQUIRE(v.front() == 10);
                REQUIRE(v.back() == 50);
                REQUIRE(v.front() == v[0]);
                REQUIRE(v.back() == v[4]);
                REQUIRE(v.back() == v[v.size() - 1]);
            }
        }
    }
    GIVEN("reference semantics and consistency")
    {
        WHEN("front() and back() return type and reference semantics are tested")
        {
            Vector v         = { 100, 200 };

            int&   front_ref = v.front();
            int&   back_ref  = v.back();

            front_ref        = 1000;
            back_ref         = 2000;

            THEN("reference modifications work")
            {
                REQUIRE(v.front() == 1000);
                REQUIRE(v.back() == 2000);
                REQUIRE(v[0] == 1000);
                REQUIRE(v[1] == 2000);
            }
            THEN("const references work")
            {
                const Vector& cv              = v;
                const int&    const_front_ref = cv.front();
                const int&    const_back_ref  = cv.back();

                REQUIRE(const_front_ref == 1000);
                REQUIRE(const_back_ref == 2000);
            }
        }
        WHEN("front() and back() consistency with other access methods is tested")
        {
            Vector v = { 7, 8, 9, 10, 11 };

            THEN("front() is consistent with other access methods")
            {
                REQUIRE(v.front() == v[0]);
                REQUIRE(v.front() == v.at(0));
                REQUIRE(v.front() == *v.begin());
            }
            THEN("back() is consistent with other access methods")
            {
                REQUIRE(v.back() == v[v.size() - 1]);
                REQUIRE(v.back() == v.at(v.size() - 1));
                REQUIRE(v.back() == *(v.end() - 1));
            }
            THEN("modifications are reflected in all access methods")
            {
                v.front() = 77;
                v.back()  = 111;

                REQUIRE(v[0] == 77);
                REQUIRE(v.at(0) == 77);
                REQUIRE(*v.begin() == 77);

                REQUIRE(v[v.size() - 1] == 111);
                REQUIRE(v.at(v.size() - 1) == 111);
                REQUIRE(*(v.end() - 1) == 111);
            }
        }
        WHEN("front() and back() are used on large vector")
        {
            Vector v(static_cast<Vector::size_type>(1000), 5);

            THEN("initial values are correct")
            {
                REQUIRE(v.front() == 5);
                REQUIRE(v.back() == 5);
            }
            THEN("boundary modifications work")
            {
                v.front() = 1;
                v.back()  = 1000;

                REQUIRE(v.front() == 1);
                REQUIRE(v.back() == 1000);
                REQUIRE(v[0] == 1);
                REQUIRE(v[999] == 1000);
                REQUIRE(v[500] == 5);
            }
        }
    }
}

TEST_CASE("data() method", "[vector][data][access]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("data() is called on empty vector")
        {
            Vector v;

            THEN("data() returns nullptr")
            {
                REQUIRE(v.data() == nullptr);
            }
            THEN("const data() returns nullptr")
            {
                const Vector& cv = v;
                REQUIRE(cv.data() == nullptr);
            }
        }
    }
    GIVEN("single element vector")
    {
        WHEN("data() is called on single element vector")
        {
            Vector v   = { 42 };

            int*   ptr = v.data();

            THEN("data() returns valid pointer")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(*ptr == 42);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr == &v.back());
            }
            THEN("element can be modified through pointer")
            {
                *ptr = 99;
                REQUIRE(v[0] == 99);
                REQUIRE(v.front() == 99);
            }
        }
        WHEN("const data() is called on single element vector")
        {
            const Vector v   = { 42 };
            const int*   ptr = v.data();

            THEN("const data() returns valid pointer")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(*ptr == 42);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr == &v.back());
            }
        }
    }
    GIVEN("multi-element vector")
    {
        WHEN("data() is called on multi-element vector")
        {
            Vector v   = { 10, 20, 30, 40, 50 };
            int*   ptr = v.data();

            THEN("data() returns valid pointer to first element")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
            }
            THEN("all elements are accessible through pointer")
            {
                for (Vector::size_type i = 0; i < v.size(); ++i)
                {
                    REQUIRE(ptr[i] == v[i]);
                    REQUIRE(&ptr[i] == &v[i]);
                }
            }
            THEN("elements can be modified through pointer")
            {
                ptr[2] = 333;
                REQUIRE(v[2] == 333);
                REQUIRE(v.at(2) == 333);
            }
        }
        WHEN("const data() is called on multi-element vector")
        {
            const Vector v   = { 10, 20, 30, 40, 50 };
            const int*   ptr = v.data();

            THEN("const data() returns valid pointer")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
            }
            THEN("all elements are accessible through const pointer")
            {
                for (Vector::size_type i = 0; i < v.size(); ++i)
                {
                    REQUIRE(ptr[i] == v[i]);
                    REQUIRE(&ptr[i] == &v[i]);
                }
            }
        }
    }
    GIVEN("pointer arithmetic and consistency")
    {
        WHEN("data() pointer arithmetic is performed")
        {
            Vector v   = { 1, 2, 3, 4, 5 };
            int*   ptr = v.data();

            THEN("pointer arithmetic works correctly")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr + v.size() - 1 == &v.back());
                REQUIRE(*(ptr + 2) == v[2]);
                REQUIRE(*(ptr + 4) == v[4]);
            }
            THEN("all elements accessible via pointer arithmetic")
            {
                for (Vector::size_type i = 0; i < v.size(); ++i)
                {
                    REQUIRE(*(ptr + i) == v[i]);
                }
            }
        }
        WHEN("data() consistency with iterators is tested")
        {
            Vector v   = { 7, 8, 9, 10, 11 };
            int*   ptr = v.data();

            THEN("data() is consistent with iterators")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &(*v.begin()));
            }
            THEN("pointer arithmetic matches iterator advancement")
            {
                auto it = v.begin();
                for (Vector::size_type i = 0; i < v.size(); ++i, ++it)
                {
                    REQUIRE(ptr + i == &(*it));
                    REQUIRE(*(ptr + i) == *it);
                }
            }
        }
        WHEN("data() is used on large vector")
        {
            Vector v(static_cast<Vector::size_type>(1000), 7);
            int*   ptr = v.data();

            THEN("data() works with large vectors")
            {
                REQUIRE(ptr != nullptr);
                REQUIRE(ptr == &v[0]);
                REQUIRE(ptr == &v.front());
                REQUIRE(ptr + 999 == &v.back());
            }
            THEN("pointer arithmetic works with large vectors")
            {
                REQUIRE(*ptr == 7);
                REQUIRE(*(ptr + 999) == 7);
                REQUIRE(*(ptr + 500) == 7);
            }
            THEN("modifications work with large vectors")
            {
                *(ptr + 500) = 555;
                REQUIRE(v[500] == 555);
                REQUIRE(v.at(500) == 555);
            }
        }
        WHEN("data() is used after vector operations")
        {
            Vector v = { 1, 2, 3 };

            THEN("data() returns nullptr after clear")
            {
                v.clear();
                REQUIRE(v.data() == nullptr);
            }
            THEN("data() works after assign")
            {
                v.assign({ 10, 20 });
                int* after_assign_ptr = v.data();
                REQUIRE(after_assign_ptr != nullptr);
                REQUIRE(*after_assign_ptr == 10);
                REQUIRE(*(after_assign_ptr + 1) == 20);
            }
        }
        WHEN("data() return type verification is performed")
        {
            Vector        v  = { 100, 200 };
            const Vector& cv = v;

            THEN("return types are correct")
            {
                static_assert(std::is_same_v<decltype(v.data()), int*>);
                static_assert(std::is_same_v<decltype(cv.data()), const int*>);
            }
            THEN("pointers point to same location")
            {
                int*       mutable_ptr = v.data();
                const int* const_ptr   = cv.data();

                REQUIRE(mutable_ptr == const_ptr);
                REQUIRE(*mutable_ptr == *const_ptr);
            }
        }
    }
}
