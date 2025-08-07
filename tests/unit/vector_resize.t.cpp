#include <test_copyconstructible.h>
#include <test_defaultcopyconstructible.h>
#include <test_permute.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

/*
 * Type requirements
 * - resize(size_type count): MoveInsertable and DefaultInsertable
 * - resize(size_type count, const value_type& value): CopyInsertable
 */
TEMPLATE_LIST_TEST_CASE("Test resize()", "[vector][resize]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("resize() is 0")
        {
            size_t const size = 0;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
                REQUIRE(v.empty());
            }
        }
        WHEN("resize() increases size")
        {
            size_t const size = 3;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
                REQUIRE(!v.empty());
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
        WHEN("resize() to large size")
        {
            size_t const size = 1000;

            v.resize(size);
            THEN("size is changed correctly")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
            THEN("all elements are default-constructed")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        size_t const count = 5;
        value_type   first_value;
        value_type   value;
        Vector       values;

        for (size_t i = 0; i < count; ++i)
        {
            pw::test::permute(value, 10);
            if (i == 0)
                first_value = value;
            values.push_back(value);
        }
        Vector v(values);

        WHEN("resize() is 0")
        {
            size_t const size = 0;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
                REQUIRE(v.empty());
            }
        }
        WHEN("resize() increases size")
        {
            size_t const size = count + 3;

            v.resize(size);
            THEN("size is changed")
            {
                REQUIRE(size == v.size());
                REQUIRE(size <= v.capacity());
            }
            THEN("original elements are preserved")
            {
                for (size_t i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == values[i]);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = count; i < size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
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
            THEN("remaining elements are preserved")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == values[i]);
                }
            }
        }
        WHEN("resize() decreases to 1 element")
        {
            size_t const capacity = v.capacity();
            size_t const size     = 1;

            v.resize(size);
            THEN("size is 1")
            {
                REQUIRE(size == v.size());
                REQUIRE(!v.empty());
            }
            THEN("capacity is unchanged")
            {
                REQUIRE(capacity == v.capacity());
            }
            THEN("first element is preserved")
            {
                REQUIRE(v[0] == first_value);
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
            THEN("all elements are preserved")
            {
                for (size_t i = 0; i < size; ++i)
                {
                    REQUIRE(v[i] == values[i]);
                }
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Test resize() edge cases", "[vector][resize][edge]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("A vector at maximum capacity")
    {
        Vector v;
        v.reserve(10);
        for (size_t i = 0; i < 10; ++i)
        {
            value_type val;
            pw::test::permute(val, i);
            v.push_back(val);
        }

        WHEN("resize() to current size")
        {
            size_t const original_size     = v.size();
            size_t const original_capacity = v.capacity();

            v.resize(original_size);
            THEN("nothing changes")
            {
                REQUIRE(v.size() == original_size);
                REQUIRE(v.capacity() == original_capacity);
            }
        }

        WHEN("resize() to smaller size")
        {
            size_t const new_size          = 5;
            size_t const original_capacity = v.capacity();
            Vector       original_values(v);

            v.resize(new_size);
            THEN("size decreases, capacity unchanged")
            {
                REQUIRE(v.size() == new_size);
                REQUIRE(v.capacity() == original_capacity);
            }
            THEN("remaining elements are unchanged")
            {
                for (size_t i = 0; i < new_size; ++i)
                {
                    REQUIRE(v[i] == original_values[i]);
                }
            }
        }
    }

    GIVEN("A vector with capacity > size")
    {
        Vector v;
        v.reserve(20);
        for (size_t i = 0; i < 5; ++i)
        {
            value_type val;
            pw::test::permute(val, i);
            v.push_back(val);
        }

        WHEN("resize() within capacity")
        {
            size_t const original_capacity = v.capacity();
            size_t const new_size          = 15;
            Vector       original_values(v);

            v.resize(new_size);
            THEN("size increases, capacity unchanged")
            {
                REQUIRE(v.size() == new_size);
                REQUIRE(v.capacity() == original_capacity);
            }
            THEN("original elements preserved")
            {
                for (size_t i = 0; i < original_values.size(); ++i)
                {
                    REQUIRE(v[i] == original_values[i]);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = original_values.size(); i < new_size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }

        WHEN("resize() beyond capacity")
        {
            size_t const original_capacity = v.capacity();
            size_t const new_size          = original_capacity + 10;
            Vector       original_values(v);

            v.resize(new_size);
            THEN("size increases, capacity increases")
            {
                REQUIRE(v.size() == new_size);
                REQUIRE(v.capacity() >= new_size);
                REQUIRE(v.capacity() > original_capacity);
            }
            THEN("original elements preserved")
            {
                for (size_t i = 0; i < original_values.size(); ++i)
                {
                    REQUIRE(v[i] == original_values[i]);
                }
            }
            THEN("new elements are default-constructed")
            {
                for (size_t i = original_values.size(); i < new_size; ++i)
                {
                    REQUIRE(v[i] == value_type {});
                }
            }
        }
    }
}
SCENARIO("resize() op counts", "[vector][resize][optracker]")
{
    using Vector     = pw::vector<pw::test::DefaultCopyConstructible>;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init = pw::test::DefaultCopyConstructible::getCounter();

    GIVEN("An empty vector")
    {
        Vector v;

        WHEN("resize() is called")
        {
            size_t count = 5;
            counter      = pw::test::DefaultCopyConstructible::getCounter();

            v.resize(count);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
            THEN("default constructed count times")
            {
                INFO("counter: " << counter);
                REQUIRE(count == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() == counter.constructorCount());
                REQUIRE(counter.allCount() == count);
            }
        }
    }

    GIVEN("A vector with existing elements")
    {
        Vector v { 1, 2, 3 };

        WHEN("resize() increases size")
        {
            counter = pw::test::DefaultCopyConstructible::getCounter();
            v.resize(v.size() + 2);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
            THEN("new elements are default constructed and original copy constructed")
            {
                // Expect copy construction so exceptions leave unchanged
                INFO("counter: " << counter);
                REQUIRE(2 == counter.getDefaultConstructor());
                REQUIRE(counter.getDefaultConstructor() + counter.getCopyConstructor() ==
                        counter.constructorCount());
            }
        }

        WHEN("resize() decreases size")
        {
            pw::test::OpCounter before = pw::test::DefaultCopyConstructible::getCounter();
            v.resize(1);
            counter = pw::test::DefaultCopyConstructible::getCounter() - before;
            THEN("excess elements are destructed")
            {
                INFO("counter: " << counter);
                REQUIRE(0 == counter.constructorCount());
                REQUIRE(2 == counter.destructorCount());
            }
        }

        WHEN("resize() to 0")
        {
            pw::test::OpCounter before = pw::test::DefaultCopyConstructible::getCounter();
            v.resize(0);
            counter = pw::test::DefaultCopyConstructible::getCounter() - before;
            THEN("all elements are destructed")
            {
                INFO("counter: " << counter);
                REQUIRE(0 == counter.constructorCount());
                REQUIRE(3 == counter.destructorCount());
            }
        }
    }
    counter = pw::test::DefaultCopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

SCENARIO("resize() stress tests", "[vector][resize][stress]")
{
    using Vector = pw::vector<int>;

    GIVEN("A vector")
    {
        Vector v;

        WHEN("multiple resize operations are performed")
        {
            for (size_t i = 0; i < 100; ++i)
            {
                v.resize(i);
                REQUIRE(v.size() == i);
                for (int j : v)
                {
                    REQUIRE(j == 0);
                }
            }

            THEN("all operations succeed")
            {
                REQUIRE(v.size() == 99);
            }

            WHEN("resize down and up repeatedly")
            {
                for (size_t i = 0; i < 10; ++i)
                {
                    v.resize(50);
                    REQUIRE(v.size() == 50);
                    v.resize(100);
                    REQUIRE(v.size() == 100);
                }

                THEN("final state is correct")
                {
                    REQUIRE(v.size() == 100);
                    for (int j : v)
                    {
                        REQUIRE(j == 0);
                    }
                }
            }
        }
    }
}
