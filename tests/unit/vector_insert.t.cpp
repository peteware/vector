
#include <test_defaultcopyconstructible.h>
#include <test_permute.h>
#include <test_same.h>
#include <test_testtype.h>
#include <test_values.h>

#include <pw/algorithm>

#include <catch2/catch.hpp>

#include <iostream>
#include <iterator>

/*
 * Type requirements:
 * - insert(const_iterator pos, const T& value): CopyAssignable and CopyInsertable
 * - insert(const_iterator pos, T&& value):  MoveAssignable and MoveInsertable
 * - insert(const_iterator pos, size_type count, const T& value):  CopyAssignable and CopyInsertable
 * - insert(const_iterator pos, InputIt first, InputIt last): EmplaceConstructible, Swappable, MoveAssignable, MoveConstructible and MoveInsertable
 * - insert(const_iterator pos, std::initializer_list<T> ilist): EmplaceConstructible, Swappable, MoveAssignable, MoveConstructible and MoveInsertable 
 *
 * Exceptions:
 * - yex
 */
TEMPLATE_LIST_TEST_CASE("Test insert(pos, value)", "[vector][insert]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("insert(begin, value) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 3);
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
        WHEN("insert(end, value) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 3);
            iter = v.insert(v.end(), value);
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
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector>  generate(5);
        size_t                    added;
        size_t                    offset;
        value_type                value;
        typename Vector::iterator where;

        WHEN("insert() at begin")
        {
            generate.values.insert(generate.values.begin(), value);
            THEN("size() is bigger")
            {
                REQUIRE(generate.values.size() == generate.count + 1);
            }
            THEN("inserted value at front")
            {
                REQUIRE(value == generate.values.front());
            }
            THEN("moved to 1 position")
            {
                REQUIRE(generate.first_value == generate.values[1]);
            }
            THEN("last is still last")
            {
                REQUIRE(generate.last_value == generate.values.back());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Test insert(pos, count, value)", "[vector][insert]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("insert(begin(), count, value)")
        {
            typename Vector::iterator iter;
            value_type                value;
            size_t                    count = 12;

            pw::test::permute(value, 3);
            iter = v.insert(v.begin(), count, value);
            THEN("there are count values")
            {
                REQUIRE(v.begin() == iter);
                REQUIRE(count == v.size());
                REQUIRE(value == v[0]);
                REQUIRE(value == v[count - 1]);
            }
        }
        WHEN("insert(end(), count, value)")
        {
            typename Vector::iterator iter;
            value_type                value;
            size_t                    count = 12;

            pw::test::permute(value, 3);
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
        pw::test::Values<Vector>  generate(5);
        size_t                    added;
        size_t                    offset;
        value_type                value;
        typename Vector::iterator where;

        Vector                    v(generate.values);
        WHEN("insert(begin(), added, value)")
        {
            added  = 3;
            offset = 0;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(end(), added, value)")
        {
            added  = 3;
            offset = v.size();
            where  = v.insert(v.end(), added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(middle, added, value)")
        {
            added  = 10;
            offset = v.size() / 2;
            where  = v.insert(v.begin() + offset, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
        WHEN("insert(end -1, added, value)")
        {
            added  = 10;
            offset = generate.count - 1;
            where  = v.begin() + offset;
            where  = v.insert(where, added, value);
            THEN("size is correct")
            {
                REQUIRE(generate.count + added == v.size());
            }
            THEN("items are the same")
            {
                REQUIRE(pw::test::same(where, where + added, value));
                REQUIRE(
                    pw::equal(generate.values.begin(), generate.values.begin() + offset, v.begin(), where));
                REQUIRE(pw::equal(
                    generate.values.begin() + offset, generate.values.end(), where + added, v.end()));
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Test insert(pos, first, last)", "[vector][insert][test]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    GIVEN("an empty vector")
    {
        Vector                   v;
        pw::test::Values<Vector> generate(5);

        WHEN("insert(begin, first, last) is called without capacity")
        {
            typename Vector::iterator iter;
            iter = v.insert(v.begin(), generate.values.begin(), generate.values.end());
            THEN("size() is increased")
            {
                REQUIRE(generate.values.size() == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
            }
            THEN("front() returns same value")
            {
                REQUIRE(generate.first_value == v.front());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector                   v(generate.values);
        WHEN("insert(begin, first, last) is called without capacity")
        {
            typename Vector::iterator iter;
            iter = v.insert(v.begin(), generate.values.begin(), generate.values.end());
            THEN("size() is increased")
            {
                REQUIRE(2 * generate.values.size() == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
            }
            THEN("front() returns same value")
            {
                REQUIRE(generate.first_value == v.front());
            }
        }
        WHEN("insert(end, first, last) is called without capacity")
        {
            typename Vector::iterator iter;
            iter = v.insert(v.end(), generate.values.begin(), generate.values.end());
            THEN("size() is increased")
            {
                REQUIRE(2 * generate.values.size() == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(pw::next(v.begin(), generate.values.size()) == iter);
            }
            THEN("front() returns same value")
            {
                REQUIRE(generate.last_value == v.back());
            }
        }
        WHEN("insert(begin, first, last) is called with enough capacity")
        {
            typename Vector::size_type space;
            typename Vector::iterator  iter;
            v.reserve(v.size() + generate.values.size());
            space = v.capacity();
            iter  = v.insert(v.begin(), generate.values.begin(), generate.values.end());
            THEN("capacity() is unchanged")
            {
                REQUIRE(space == v.capacity());
            }
            THEN("size() is increased")
            {
                REQUIRE(2 * generate.values.size() == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
            }
            THEN("inserted values match")
            {
                REQUIRE(generate.first_value == v[0]);
            }
            THEN("back() returns same value")
            {
                REQUIRE(generate.last_value == v.back());
            }
        }
        WHEN("insert(end, first, last) is called with enough capacity")
        {
            typename Vector::iterator iter;
            v.reserve(v.size() + generate.values.size());
            iter = v.insert(v.end(), generate.values.begin(), generate.values.end());
            THEN("size() is increased")
            {
                REQUIRE(2 * generate.values.size() == v.size());
            }
            THEN("iter is at start of insert")
            {
                REQUIRE(v.begin() + generate.values.size() == iter);
            }
            THEN("back() returns same value")
            {
                REQUIRE(generate.last_value == v.back());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Test insert(pos, value&&)", "[vector][insert][move]", pw::test::TestTypeList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    
    GIVEN("an empty vector")
    {
        Vector v;

        WHEN("insert(begin(), std::move(value)) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 3);
            value_type moved_value = value;
            iter = v.insert(v.begin(), std::move(moved_value));
            
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
        
        WHEN("insert(end(), std::move(value)) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 3);
            value_type moved_value = value;
            iter = v.insert(v.end(), std::move(moved_value));
            
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
    }
    
    GIVEN("a vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector v(generate.values);
        
        WHEN("insert(begin(), std::move(value)) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 7);
            value_type moved_value = value;
            size_t original_size = v.size();
            
            iter = v.insert(v.begin(), std::move(moved_value));
            
            THEN("size() is increased by 1")
            {
                REQUIRE(original_size + 1 == v.size());
            }
            THEN("begin() is same as returned iterator")
            {
                REQUIRE(v.begin() == iter);
            }
            THEN("inserted value is at front")
            {
                REQUIRE(value == v.front());
            }
            THEN("original first element moved to position 1")
            {
                REQUIRE(generate.first_value == v[1]);
            }
            THEN("original last element is still at back")
            {
                REQUIRE(generate.last_value == v.back());
            }
        }
        
        WHEN("insert(end(), std::move(value)) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 7);
            value_type moved_value = value;
            size_t original_size = v.size();
            
            iter = v.insert(v.end(), std::move(moved_value));
            
            THEN("size() is increased by 1")
            {
                REQUIRE(original_size + 1 == v.size());
            }
            THEN("returned iterator points to inserted element")
            {
                REQUIRE(v.end() - 1 == iter);
            }
            THEN("inserted value is at back")
            {
                REQUIRE(value == v.back());
            }
            THEN("original first element is still at front")
            {
                REQUIRE(generate.first_value == v.front());
            }
        }
        
        WHEN("insert(middle, std::move(value)) is called")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 7);
            value_type moved_value = value;
            size_t original_size = v.size();
            size_t insert_pos = v.size() / 2;
            
            iter = v.insert(v.begin() + insert_pos, std::move(moved_value));
            
            THEN("size() is increased by 1")
            {
                REQUIRE(original_size + 1 == v.size());
            }
            THEN("returned iterator points to inserted element")
            {
                REQUIRE(v.begin() + insert_pos == iter);
            }
            THEN("inserted value is at correct position")
            {
                REQUIRE(value == v[insert_pos]);
            }
            THEN("elements before insertion point are unchanged")
            {
                for (size_t i = 0; i < insert_pos; ++i)
                {
                    REQUIRE(generate.values[i] == v[i]);
                }
            }
            THEN("elements after insertion point are shifted")
            {
                for (size_t i = insert_pos; i < original_size; ++i)
                {
                    REQUIRE(generate.values[i] == v[i + 1]);
                }
            }
        }
    }
    
    GIVEN("a vector with sufficient capacity")
    {
        Vector v;
        v.reserve(10);
        pw::test::Values<Vector> generate(5);
        for (auto const& val : generate.values)
        {
            v.push_back(val);
        }
        
        WHEN("insert(middle, std::move(value)) is called without reallocation")
        {
            typename Vector::iterator iter;
            value_type                value;
            pw::test::permute(value, 7);
            value_type moved_value = value;
            size_t original_capacity = v.capacity();
            size_t original_size = v.size();
            size_t insert_pos = v.size() / 2;
            
            iter = v.insert(v.begin() + insert_pos, std::move(moved_value));
            
            THEN("capacity() is unchanged")
            {
                REQUIRE(original_capacity == v.capacity());
            }
            THEN("size() is increased by 1")
            {
                REQUIRE(original_size + 1 == v.size());
            }
            THEN("inserted value is at correct position")
            {
                REQUIRE(value == v[insert_pos]);
            }
        }
    }
}

SCENARIO("insert() op counts", "[vector][insert][optracker]")
{
    using Vector     = pw::vector<pw::test::DefaultCopyConstructible>;
    using value_type = typename Vector::value_type;

    pw::test::OpCounter       counter;
    pw::test::OpCounter const init  = pw::test::DefaultCopyConstructible::getCounter();
    size_t                    count = 5;

    GIVEN("An empty vector")
    {
        Vector                             v;
        pw::test::DefaultCopyConstructible copyObject;

        WHEN("insert(count) at begin")
        {
            pw::test::DefaultCopyConstructible copyObject;
            counter = pw::test::DefaultCopyConstructible::getCounter();
            v.insert(v.begin(), count, copyObject);
            THEN("Copy constructor called count times")
            {
                counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
                REQUIRE(count == counter.constructorCount());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector>           generate(5);
        pw::test::DefaultCopyConstructible copyObject;

        generate.values.shrink_to_fit();
        counter = pw::test::DefaultCopyConstructible::getCounter();
        WHEN("insert() at begin")
        {
            generate.values.insert(generate.values.begin(), copyObject);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items for more space")
            {
                REQUIRE(generate.count == counter.getMoveConstructor());
            }
            THEN("Assigned item")
            {
                REQUIRE(1 == counter.getCopyConstructor());
            }
        }
        WHEN("insert() at end")
        {
            generate.values.insert(generate.values.end(), copyObject);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items for more space")
            {
                REQUIRE(generate.count == counter.getMoveConstructor());
            }
            THEN("Copy constructed new item")
            {
                REQUIRE(1 == counter.getCopyConstructor());
            }
        }
        WHEN("insert() in middle")
        {
            generate.values.insert(generate.values.begin() + generate.count - 2, copyObject);
            counter = pw::test::DefaultCopyConstructible::getCounter() - counter;
            THEN("Move constructed existing items for more space")
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
