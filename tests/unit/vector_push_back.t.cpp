#include "pw/impl/pmr_polymorphic_allocator.h"

#include <test_allocator_move_assignment.h>
#include <test_optracker_copyconstructible.h>
#include <test_optracker_defaultcopyconstructible.h>
#include <test_permute.h>
#include <test_testtype.h>
#include <test_values.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

/*
 * Type requirements
 * - push_back(const T& value): CopyInsertable
 * - push_back(T&& value): MoveInsertable
 */
TEMPLATE_LIST_TEST_CASE("push_back()", "[vector][push_back]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector     v;
        value_type value;
        pw::test::permute(value, 1);
        WHEN("push_back() is called")
        {
            v.push_back(value);
            THEN("capacity() is increased")
            {
                REQUIRE(v.capacity() == 1);
            }
            THEN("value is there")
            {
                REQUIRE(value == v.front());
                REQUIRE(value == v.back());
            }
        }
        WHEN("push_back() is called N times")
        {
            value_type const orig  = value;
            value_type       last  = value;
            constexpr int    total = 20;
            for (int i = 0; i < total; ++i)
            {
                last = value;
                v.push_back(value);
                pw::test::permute(value, i);
            }
            THEN("size() is correct")
            {
                REQUIRE(v.size() == total);
            }
            THEN("capacity() is increased")
            {
                REQUIRE(v.capacity() >= total);
            }
            THEN("value is there")
            {
                REQUIRE(orig == v.front());
                REQUIRE(last == v.back());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        constexpr size_t         count = 5;
        pw::test::Values<Vector> generate(count);
        Vector&                  v = generate.values;

        WHEN("push_back() const_ref is called to exceed capacity")
        {
            size_t capacity = v.capacity();
            for (size_t i = 0; i < capacity; ++i)
            {
                value_type t {};
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
    using Vector                   = pw::pmr::vector<pw::test::OpTrackerDefaultCopyConstructible>;
    using value_type               = Vector::value_type;
    pw::test::OpCounter const init = value_type::getCounter();
    pw::test::OpCounter       counter;

    GIVEN("An empty vector")
    {
        Vector     v;
        value_type copyObject(11);

        WHEN("push_back() is called")
        {
            counter = value_type::getCounter();
            v.push_back(copyObject);
            THEN("Copy construct called once")
            {
                INFO("counter: " << counter << " getCounter(): " << value_type::getCounter());
                counter = value_type::getCounter() - counter;
                REQUIRE(1 == counter.constructorCount());
                REQUIRE(counter.constructorCount() == counter.allCount());
            }
        }
        WHEN("push_back(move) is called")
        {
            SKIP();
            counter = value_type::getCounter();
            v.push_back(pw::move(copyObject));
            THEN("Copy construct called once")
            {
                counter = value_type::getCounter() - counter;
                REQUIRE(1 == counter.getMoveConstructor());
                REQUIRE(counter.getMoveConstructor() == counter.allCount());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        SKIP();
        pw::test::Values<Vector> generate(5);
        value_type               copyObject(12);

        generate.values.shrink_to_fit();
        counter = copyObject.opCounter();
        WHEN("push_back() is called")
        {
            generate.values.push_back(copyObject);
            counter = copyObject.opCounter() - counter;
            THEN("Copy constructed existing items and new item")
            {
                INFO("counter: " << counter);
                REQUIRE(generate.count + 1 == counter.getCopyConstructor());
            }
        }
    }
    counter = value_type::getCounter() - init;
    INFO("counter: " << counter << " init: " << init);
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

TEST_CASE("push_back() method", "[vector][push_back][modifiers]")
{
    using Vector = pw::vector<int>;

    GIVEN("empty vector")
    {
        WHEN("push_back() is called")
        {
            Vector v;
            v.push_back(42);

            THEN("size increases to 1")
            {
                REQUIRE(v.size() == 1);
            }
            THEN("element is accessible through all access methods")
            {
                REQUIRE(v[0] == 42);
                REQUIRE(v.front() == 42);
                REQUIRE(v.back() == 42);
                REQUIRE(v.at(0) == 42);
            }
        }
        WHEN("multiple push_back() calls are made")
        {
            Vector v;
            v.push_back(10);
            v.push_back(20);
            v.push_back(30);

            THEN("size increases correctly")
            {
                REQUIRE(v.size() == 3);
            }
            THEN("all elements are stored correctly")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
                REQUIRE(v.front() == 10);
                REQUIRE(v.back() == 30);
            }
        }
    }
    GIVEN("non-empty vector")
    {
        WHEN("push_back() is called")
        {
            Vector v             = { 1, 2, 3 };
            auto   original_size = v.size();
            v.push_back(4);

            THEN("size increases by one")
            {
                REQUIRE(v.size() == original_size + 1);
            }
            THEN("existing elements are preserved")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
            THEN("new element is added at the end")
            {
                REQUIRE(v[3] == 4);
                REQUIRE(v.back() == 4);
            }
        }
    }
    GIVEN("capacity and growth scenarios")
    {
        WHEN("push_back() causes capacity growth")
        {
            Vector v;

            // Add elements until capacity grows
            for (int i = 0; i < 10; ++i)
            {
                v.push_back(i);
            }

            THEN("size increases correctly")
            {
                REQUIRE(v.size() == 10);
            }
            THEN("capacity is sufficient")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("all elements are stored correctly")
            {
                for (int i = 0; i < 10; ++i)
                {
                    REQUIRE(v[i] == i);
                }
            }
        }
        WHEN("push_back() preserves existing elements")
        {
            Vector v = { 100, 200, 300 };

            for (int i = 0; i < 50; ++i)
            {
                v.push_back(i + 1000);
            }

            THEN("size increases correctly")
            {
                REQUIRE(v.size() == 53);
            }
            THEN("original elements are preserved")
            {
                REQUIRE(v[0] == 100);
                REQUIRE(v[1] == 200);
                REQUIRE(v[2] == 300);
            }
            THEN("new elements are added correctly")
            {
                for (int i = 0; i < 50; ++i)
                {
                    REQUIRE(v[i + 3] == i + 1000);
                }
            }
        }
    }
    GIVEN("copy semantics and value handling")
    {
        WHEN("push_back() with copy semantics")
        {
            Vector v;
            int    value = 99;
            v.push_back(value);

            THEN("element is copied correctly")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v[0] == 99);
            }

            value = 77; // Modify original
            THEN("vector is unchanged when original is modified")
            {
                REQUIRE(v[0] == 99);
            }

            v[0] = 88; // Modify vector element
            THEN("original is unchanged when vector element is modified")
            {
                REQUIRE(value == 77);
                REQUIRE(v[0] == 88);
            }
        }
        WHEN("push_back() with different values")
        {
            Vector v;
            v.push_back(0);
            v.push_back(-1);
            v.push_back(2147483647);  // max int
            v.push_back(-2147483648); // min int

            THEN("all values are stored correctly")
            {
                REQUIRE(v.size() == 4);
                REQUIRE(v[0] == 0);
                REQUIRE(v[1] == -1);
                REQUIRE(v[2] == 2147483647);
                REQUIRE(v[3] == -2147483648);
            }
        }
    }
    GIVEN("iterator and capacity scenarios")
    {
        WHEN("push_back() effects on iterators")
        {
            Vector v               = { 1, 2, 3 };

            auto   begin_before    = v.begin();
            auto   end_before      = v.end();
            auto   distance_before = end_before - begin_before;
            REQUIRE(distance_before == 3);

            v.push_back(4);

            auto begin_after    = v.begin();
            auto end_after      = v.end();
            auto distance_after = end_after - begin_after;

            THEN("iterator distance increases")
            {
                REQUIRE(distance_after == 4);
            }
            THEN("new iterators are valid")
            {
                // Note: iterators from before push_back may be invalidated
                // We only test the new iterators
                REQUIRE(*begin_after == 1);
                REQUIRE(*(end_after - 1) == 4);
            }
        }
        WHEN("push_back() with reserved capacity")
        {
            Vector v;
            v.reserve(100);
            auto reserved_capacity = v.capacity();

            for (int i = 0; i < 50; ++i)
            {
                v.push_back(i);
            }

            THEN("capacity remains unchanged")
            {
                REQUIRE(v.size() == 50);
                REQUIRE(v.capacity() == reserved_capacity); // Should not reallocate
            }
            THEN("all elements are stored correctly")
            {
                for (int i = 0; i < 50; ++i)
                {
                    REQUIRE(v[i] == i);
                }
            }
        }
    }
    GIVEN("consistency and special scenarios")
    {
        WHEN("push_back() consistency with other methods")
        {
            Vector v;

            for (int i = 1; i <= 5; ++i)
            {
                v.push_back(i * 10);

                REQUIRE(v.size() == static_cast<Vector::size_type>(i));
                REQUIRE(v.back() == i * 10);
                REQUIRE(v[i - 1] == i * 10);
                REQUIRE(v.at(i - 1) == i * 10);
                REQUIRE(*(v.end() - 1) == i * 10);

                if (v.size() > 1)
                {
                    REQUIRE(v.front() == 10);
                }
            }

            THEN("all access methods return consistent values")
            {
                REQUIRE(v.size() == 5);
            }
        }
        WHEN("push_back() with different allocators")
        {
            using Allocator = pw::test::allocator_move_assignment<int>;
            using Vector    = pw::vector<int, Allocator>;

            Allocator alloc(5);
            Vector    v(alloc);

            v.push_back(10);
            v.push_back(20);
            v.push_back(30);

            THEN("allocator is preserved")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("elements are stored correctly")
            {
                REQUIRE(v[0] == 10);
                REQUIRE(v[1] == 20);
                REQUIRE(v[2] == 30);
            }
        }
        WHEN("push_back() large number of elements")
        {
            Vector        v;
            constexpr int count = 1000;

            for (int i = 0; i < count; ++i)
            {
                v.push_back(i * 2);
            }

            THEN("size and capacity are correct")
            {
                REQUIRE(v.size() == count);
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("all elements are stored correctly")
            {
                for (int i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == i * 2);
                }
                REQUIRE(v.front() == 0);
                REQUIRE(v.back() == (count - 1) * 2);
            }
        }
        WHEN("push_back() after clear")
        {
            Vector v = { 1, 2, 3, 4, 5 };

            v.clear();
            REQUIRE(v.empty());
            v.push_back(100);

            THEN("vector has one element")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v[0] == 100);
                REQUIRE(v.front() == 100);
                REQUIRE(v.back() == 100);
            }
        }
        WHEN("push_back() mixed with other operations")
        {
            Vector v;

            v.push_back(1);
            v.push_back(2);
            REQUIRE(v.size() == 2);

            v.assign({ 10, 20, 30 });
            REQUIRE(v.size() == 3);
            REQUIRE(v[0] == 10);

            v.push_back(40);
            v.clear();
            v.push_back(99);

            THEN("final state is correct")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v[0] == 99);
            }
        }
    }
}
