#include <pw/algorithm>
#include <pw/vector>
#include <test_allocator_base.h>
#include <test_input_iterator.h>
#include <test_testtype.h>

#include <initializer_list>
#include <tuple>
#include <vector>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_LIST_TEST_CASE("Constructors with int",
                        "[phase1][vector][constructor]",
                        pw::test::Phase1TestTypeList)
{
    using Vector     = TestType;
    using value_type = Vector::value_type;
    using size_type  = Vector::size_type;

    GIVEN("Construct with initializer_list")
    {
        WHEN("constructed with empty initializer list")
        {
            Vector v {};

            THEN("the vector is empty")
            {
                REQUIRE(v.empty() == true);
                REQUIRE(v.begin() == v.end());
            }
        }
        WHEN("constructed with {1, 2, 3}")
        {
            Vector v { 1, 2, 3 };

            THEN("the vector has the expected elements")
            {
                REQUIRE(v.size() == 3);
                REQUIRE(v.empty() == false);
                REQUIRE(v.capacity() >= 3);
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
        WHEN("constructed with {42}")
        {
            Vector v { 42 };

            THEN("the vector has one element")
            {
                REQUIRE(v.size() == 1);
                REQUIRE(v.empty() == false);
                REQUIRE(v.capacity() >= 1);
                REQUIRE(v[0] == 42);
            }
        }
        WHEN("constructed with {-1, 0, 1, -5, 100}")
        {
            Vector v { -1, 0, 1, -5, 100 };

            THEN("the vector has all the elements in order")
            {
                REQUIRE(v.size() == 5);
                REQUIRE(v[0] == -1);
                REQUIRE(v[1] == 0);
                REQUIRE(v[2] == 1);
                REQUIRE(v[3] == -5);
                REQUIRE(v[4] == 100);
            }
        }
    }
    GIVEN("Construct by default")
    {
        WHEN("default constructed")
        {
            Vector v;
            THEN("empty() is true")
            {
                REQUIRE(v.empty());
            }
            THEN("size() is 0")
            {
                REQUIRE(v.size() == 0); // NOLINT(*-container-size-empty)
            }
            THEN("begin() is end()")
            {
                REQUIRE(v.begin() == v.end());
            }
        }
    }
    GIVEN("Construct with allocator")
    {
        typename Vector::allocator_type alloc;
        Vector                          valloc(alloc);

        /*
         * The phase2 tests are better for as they use the allocator
         * to allocate memory and check that the allocator is used.
         * This effectively just checks the constructor exists.
         */
        THEN("the vector is empty")
        {
            REQUIRE(valloc.empty() == true);
            REQUIRE(valloc.begin() == valloc.end());
        }
        THEN("the allocator is the same as provided")
        {
            REQUIRE(valloc.get_allocator() == alloc);
        }
    }
    GIVEN("Construct with count")
    {
        WHEN("constructed with count 0")
        {
            Vector v(0);
            THEN("the vector is empty")
            {
                Vector expected {};
                REQUIRE(v.empty() == true);
                REQUIRE(v.begin() == v.end());
                REQUIRE(v == expected);
            }
        }
        WHEN("constructed with count 10")
        {
            constexpr size_type  total = 10;
            constexpr value_type value {};
            Vector               v(total);
            THEN("empty() is false")
            {
                REQUIRE(!v.empty());
            }
            THEN("size() is total")
            {
                REQUIRE(total == v.size());
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("begin() returns element")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(*(v.end() - 1) == value);
            }
            THEN("all elements have correct value")
            {
                for (size_type i = 0; i < total; ++i)
                {
                    REQUIRE(v[i] == value);
                }
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() < v.end());
                REQUIRE(v.end() - v.begin() == static_cast<typename Vector::difference_type>(total));
            }
        }
        WHEN("constructed with count 1")
        {
            Vector v(1);
            THEN("the vector has 1 default-constructed element")
            {
                Vector expected { 0 };
                REQUIRE(v == expected);
            }
            THEN("size() is 1")
            {
                REQUIRE(v.size() == 1);
            }
            THEN("it is empty")
            {
                REQUIRE(v.empty() == false);
            }
            THEN("capacity() >= 1")
            {
                REQUIRE(v.capacity() >= 1);
            }
        }
    }
    GIVEN("Construct with (count, value)")
    {
        WHEN("constructed with count=10, value=3)")
        {
            constexpr size_type  count = 10;
            constexpr value_type value = 3;
            Vector               v(count, value);

            THEN("empty() is false")
            {
                REQUIRE(!v.empty());
            }
            THEN("size() is total")
            {
                REQUIRE(count == v.size());
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("begin() returns element")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(*(v.end() - 1) == value);
            }
            THEN("all elements have correct value")
            {
                for (size_type i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == value);
                }
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() < v.end());
                REQUIRE(v.end() - v.begin() == static_cast<typename Vector::difference_type>(count));
            }
        }
        WHEN("constructed with size 0 and value 42")
        {
            Vector v(static_cast<Vector::size_type>(0), 42);

            THEN("the vector is empty")
            {
                Vector expected {};
                REQUIRE(v.empty() == true);
                REQUIRE(v.begin() == v.end());
                REQUIRE(v == expected);
            }
        }
        WHEN("constructed with size 3 and value 42")
        {
            Vector v(static_cast<Vector::size_type>(3), 42);

            THEN("the vector has 3 elements with value 42")
            {
                Vector expected { 42, 42, 42 };
                REQUIRE(v.size() == 3);
                REQUIRE(v.empty() == false);
                REQUIRE(v.capacity() >= 3);
                REQUIRE(v == expected);
            }
        }
        WHEN("constructed with size 1 and value -7")
        {
            Vector v(static_cast<Vector::size_type>(1), -7);

            THEN("the vector has 1 element with value -7")
            {
                Vector expected { -7 };
                REQUIRE(v.size() == 1);
                REQUIRE(v.empty() == false);
                REQUIRE(v.capacity() >= 1);
                REQUIRE(v == expected);
            }
        }
    }
    GIVEN("constexpr context")
    {
        WHEN("default constructor is used in constexpr")
        {
            constexpr auto test_constexpr = []() constexpr {
                Vector v;
                return v.empty();
            };

            THEN("compilation succeeds and runtime validation passes")
            {
                static_assert(test_constexpr());
                REQUIRE(test_constexpr());
            }
        }
        WHEN("initializer list constructor is used in constexpr")
        {
            constexpr auto test_constexpr = []() constexpr {
                Vector v { 1, 2, 3 };
                return v.size() == 3 && v[0] == 1 && v[1] == 2 && v[2] == 3;
            };

            THEN("runtime validation passes")
            {
                //static_assert(test_constexpr());
                REQUIRE(test_constexpr());
            }
        }
    }
    GIVEN("Copy constructor")
    {
        WHEN("copying an empty vector")
        {
            Vector source {};
            Vector copy(source);

            THEN("the copy is also empty")
            {
                Vector expected {};
                REQUIRE(copy.empty() == true);
                REQUIRE(copy.begin() == copy.end());
                REQUIRE(copy == expected);
                REQUIRE(copy == source);
            }
        }
        WHEN("copying a vector with elements")
        {
            Vector source { 99, 99, 99 };
            Vector copy(source); // NOLINT(*-unnecessary-copy-initialization)

            THEN("the copy has the same elements")
            {
                Vector expected { 99, 99, 99 };
                REQUIRE(copy == expected);
                REQUIRE(copy == source);
            }
        }
        WHEN("copying a vector with different values")
        {
            Vector source { 1, 2, 3, 4 };
            Vector copy(source); // NOLINT(*-unnecessary-copy-initialization)

            THEN("the copy has identical content")
            {
                Vector expected { 1, 2, 3, 4 };
                REQUIRE(copy.size() == 4);
                REQUIRE(copy == expected);
                REQUIRE(copy == source);
            }
        }
    }
    GIVEN("Copy constructor with allocator")
    {
        // See phase2 tests where allocators are used more extensively.
        WHEN("Copying an empty vector with allocator")
        {
            Vector source {};
            Vector copy(source, source.get_allocator());

            THEN("the copy is also empty")
            {
                Vector expected {};
                REQUIRE(copy.empty() == true);
            }
        }
        WHEN("Copying a vector with elements and allocator")
        {
            Vector source { 10, 20, 30 };
            Vector copy(source, source.get_allocator());

            THEN("the copy has the same elements")
            {
                Vector expected { 10, 20, 30 };
                REQUIRE(copy == expected);
            }
        }
    }
    GIVEN("Move constructor")
    {
        WHEN("moving an empty vector")
        {
            Vector source {};
            Vector original_copy(source); // Keep a copy for comparison
            Vector moved(pw::move(source));

            THEN("the moved vector is empty")
            {
                Vector expected {};
                REQUIRE(moved.empty() == true);
                REQUIRE(moved.begin() == moved.end());
                REQUIRE(moved == expected);
                REQUIRE(moved == original_copy);
            }
        }
        WHEN("moving a vector with elements")
        {
            Vector source { 10, 20, 30 };
            Vector original_copy(source); // Keep a copy for comparison
            Vector moved(std::move(source));

            THEN("the moved vector has the original content")
            {
                Vector expected { 10, 20, 30 };
                REQUIRE(moved.size() == 3);
                REQUIRE(moved.empty() == false);
                REQUIRE(moved == expected);
                REQUIRE(moved == original_copy);
            }
        }
    }
    GIVEN("Move constructor with allocator")
    {
        // See phase2 tests where allocators are used more extensively.
        WHEN("Moving an empty vector with allocator")
        {
            Vector source {};
            Vector moved(pw::move(source), source.get_allocator());

            THEN("the moved vector is also empty")
            {
                REQUIRE(moved.empty() == true);
            }
        }
        WHEN("Moving a vector with elements and allocator")
        {
            Vector source { 10, 20, 30 };
            Vector moved(pw::move(source), source.get_allocator());

            THEN("the moved vector has the original content")
            {
                Vector expected { 10, 20, 30 };
                REQUIRE(moved == expected);
            }
        }
    }
    GIVEN("Construct with range (iter1, iter2)")
    {
        WHEN("constructed from empty range")
        {
            Vector source {};
            Vector v(source.begin(), source.end());
            THEN("the vector is empty")
            {
                Vector expected {};
                REQUIRE(v.empty() == true);
                REQUIRE(v.begin() == v.end());
                REQUIRE(v == expected);
                REQUIRE(v == source);
            }
        }
        WHEN("constructed from vector range")
        {
            Vector source { 10, 20, 30, 40 };
            Vector v(source.begin(), source.end());

            THEN("the vector has the same elements")
            {
                Vector expected { 10, 20, 30, 40 };
                REQUIRE(v.size() == 4);
                REQUIRE(v.empty() == false);
                REQUIRE(v == expected);
                REQUIRE(v == source);
            }
        }
        WHEN("constructed from partial range")
        {
            Vector source { 1, 2, 3, 4, 5 };
            Vector v(source.begin() + 1, source.begin() + 4); // elements 2, 3, 4

            THEN("the vector has the partial range")
            {
                Vector expected { 2, 3, 4 };
                REQUIRE(v.size() == 3);
                REQUIRE(v == expected);
            }
        }
        WHEN("constructed from array")
        {
            int    arr[] = { 7, 8, 9 };
            Vector v(arr, arr + 3);

            THEN("the vector has the array elements")
            {
                Vector expected { 7, 8, 9 };
                REQUIRE(v.size() == 3);
                REQUIRE(v == expected);
            }
        }
        WHEN("constructed from input iterator (skipped)")
        {
            /*
             * Moved this test to phase3 as an simple implementation
             * of this is to use `push_back()` to insert each item
             * but I didn't want to have to implement `push_back()`
             * at this point.
             */
            Vector                        v { 1, 2, 3, 4, 5 };
            pw::test::test_input_iterator iter(v.begin());
            pw::test::test_input_iterator end(v.end());
            Vector                        constructed(iter, end);
            THEN("the vector has the same elements")
            {
                Vector expected { 1, 2, 3, 4, 5 };
                REQUIRE(constructed.size() == 5);
                REQUIRE(constructed == expected);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Constructors with allocator_base<int>",
                        "[phase2][vector][constructor]",
                        pw::test::Phase2TestTypeList)
{
    using Vector         = TestType;
    using value_type     = Vector::value_type;
    using size_type      = Vector::size_type;
    using allocator_type = Vector::allocator_type;
    GIVEN("default allocator instance")
    {
        WHEN("default constructor is called")
        {
            // constexpr vector() noexcept(noexcept(allocator_type()));
            Vector v;

            THEN("vector is empty")
            {
                REQUIRE(v.empty());
            }
            THEN("allocator matches default instance")
            {
                allocator_type alloc;
                REQUIRE(v.get_allocator().m_instance == alloc.m_instance);
            }
        }
    }
    GIVEN("a vector with elements { 1, 2, 3, 10 }")
    {
        Vector v1 { 1, 2, 3, 10 };
        WHEN("copy constructor is called")
        {
            // constexpr vector(vector const& other);
            Vector v2(v1);
            THEN("allocators are equal")
            {
                REQUIRE(v1.get_allocator() == v2.get_allocator());
            }
            THEN("sizes are equal")
            {
                REQUIRE(v1.size() == v2.size());
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v2.capacity() >= v2.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v2.empty());
            }
            THEN("all elements are copied correctly")
            {
                REQUIRE(v1 == v2);
            }
            THEN("iterators are valid")
            {
                REQUIRE(v2.begin() != v2.end());
            }
        }
        WHEN("move constructor is called")
        {
            // constexpr vector(vector&& other) noexcept;
            auto   original_size = v1.size();
            Vector v2(pw::move(v1));

            THEN("allocators are equal")
            {
                REQUIRE(v1.get_allocator() == v2.get_allocator());
            }
            THEN("size is preserved")
            {
                REQUIRE(v2.size() == original_size);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v2.capacity() >= v2.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v2.empty());
            }
            THEN("all elements are moved correctly")
            {
                REQUIRE(v2[0] == 1);
                REQUIRE(v2[1] == 2);
                REQUIRE(v2[2] == 3);
                REQUIRE(v2[3] == 10);
            }
            THEN("iterators are valid")
            {
                REQUIRE(v2.begin() != v2.end());
            }
        }
    }
    GIVEN("a custom allocator instance")
    {
        allocator_type alloc(3);
        WHEN("default constructor is called with allocator")
        {
            // constexpr explicit vector(allocator_type const& alloc) noexcept;
            Vector v(alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
        }
        WHEN("constructor is called with count, value, and allocator")
        {
            // constexpr vector(size_type count, value_type const& value, allocator_type const& alloc = allocator_type());
            constexpr size_type  count = 30;
            constexpr value_type value = 812;
            Vector               v(count, value, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches count")
            {
                REQUIRE(v.size() == count);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
            THEN("all elements have the specified value")
            {
                REQUIRE(v[0] == value);
                REQUIRE(v[count - 1] == value);
                for (auto i = 0; i < count; ++i)
                {
                    REQUIRE(v[i] == value);
                }
            }
        }
        WHEN("constructor is called with count and allocator but no value")
        {
            // constexpr explicit vector(size_type count, allocator_type const& alloc = allocator_type());
            constexpr size_type count = 30;
            Vector              v(count, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches count")
            {
                REQUIRE(v.size() == count);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() != v.end());
                REQUIRE(v.end() - v.begin() == static_cast<Vector::difference_type>(count));
            }
        }
        WHEN("copy constructor is called with different allocator")
        {
            // constexpr vector(vector const& other, allocator_type const& alloc);
            allocator_type alloc2;
            Vector         v1({ 1, 2, 3, 4 }, alloc);
            Vector         v2(v1, alloc2);
            THEN("new allocator is used")
            {
                REQUIRE(v2.get_allocator() == alloc2);
            }
            THEN("size is copied")
            {
                REQUIRE(v2.size() == v1.size());
            }
            THEN("elements are copied correctly")
            {
                REQUIRE(v2[0] == v1[0]);
                REQUIRE(v2[3] == v1[3]);
            }
        }
        WHEN("move constructor is called with different allocator")
        {
            // constexpr vector(vector&& other, allocator_type const& alloc);
            allocator_type alloc2;
            Vector         v1({ 1, 2, 3 }, alloc);
            Vector         v2(pw::move(v1), alloc2);
            THEN("new allocator is used")
            {
                REQUIRE(v2.get_allocator() == alloc2);
            }
            THEN("size is moved")
            {
                REQUIRE(v2.size() == v1.size());
            }
            THEN("elements are moved correctly")
            {
                REQUIRE(v2[0] == 1);
                REQUIRE(v2[2] == 3);
            }
        }
        WHEN("constructor is called with initializer list and allocator")
        {
            // constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());
            Vector v({ 1, 2, 3, 4 }, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches initializer list")
            {
                REQUIRE(v.size() == 4);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
            THEN("all elements are initialized correctly")
            {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
                REQUIRE(v[3] == 4);
            }
            THEN("iterators are valid")
            {
                REQUIRE(v.begin() != v.end());
            }
        }
        WHEN("constructor is called with iterator range and allocator")
        {
            // template<class Iterator>
            // constexpr vector(Iterator first, Iterator last, allocator_type const& alloc = allocator_type());
            value_type d[3] = { 23, 2, 1 };
            Vector     v(&d[0], &d[3], alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v.get_allocator() == alloc);
            }
            THEN("size matches range")
            {
                REQUIRE(v.size() == 3);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v.capacity() >= v.size());
            }
            THEN("elements are copied from range")
            {
                REQUIRE(v[0] == 23);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 1);
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v.empty());
            }
        }
    }
    GIVEN("constructors with zero count")
    {
        allocator_type alloc(3);

        WHEN("constructor is called with zero count and value")
        {
            constexpr value_type value = 42;
            Vector               v1(static_cast<Vector::size_type>(0), value, alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v1.get_allocator() == alloc);
            }
            THEN("vector is empty")
            {
                REQUIRE(v1.empty());
            }
            THEN("iterators are equal")
            {
                REQUIRE(v1.begin() == v1.end());
            }
        }
        WHEN("constructor is called with zero count and no value")
        {
            Vector v2(static_cast<Vector::size_type>(0), alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v2.get_allocator() == alloc);
            }
            THEN("vector is empty")
            {
                REQUIRE(v2.empty());
            }
            THEN("iterators are equal")
            {
                REQUIRE(v2.begin() == v2.end());
            }
        }
    }
    GIVEN("iterator range edge cases")
    {
        allocator_type alloc(3);
        value_type     d[] = { 1, 2, 3 };

        WHEN("constructor is called with empty range")
        {
            Vector v1(&d[0], &d[0], alloc); // first == last

            THEN("allocator is preserved")
            {
                REQUIRE(v1.get_allocator() == alloc);
            }
            THEN("vector is empty")
            {
                REQUIRE(v1.empty());
            }
            THEN("iterators are equal")
            {
                REQUIRE(v1.begin() == v1.end());
            }
        }
        WHEN("constructor is called with single element range")
        {
            Vector v2(&d[0], &d[1], alloc);

            THEN("allocator is preserved")
            {
                REQUIRE(v2.get_allocator() == alloc);
            }
            THEN("size is one")
            {
                REQUIRE(v2.size() == 1);
            }
            THEN("capacity is at least size")
            {
                REQUIRE(v2.capacity() >= v2.size());
            }
            THEN("element is copied correctly")
            {
                REQUIRE(v2[0] == 1);
            }
            THEN("vector is not empty")
            {
                REQUIRE(!v2.empty());
            }
            THEN("iterators are valid")
            {
                REQUIRE(v2.begin() != v2.end());
            }
        }
    }
    GIVEN("Two empty vectors with allocator")
    {
        Vector v1(allocator_type(5));
        Vector v2(allocator_type(10));
        WHEN("Default allocator")
        {
            THEN("allocators are different")
            {
                REQUIRE(!(v1.get_allocator() == v2.get_allocator()));
            }
        }
    }
    GIVEN("Vector with count, value, and allocator")
    {
        constexpr size_type total = 10;
        allocator_type      allocator(5);
        value_type constexpr value = 3;
        Vector v(total, value, allocator);

        WHEN("access elements")
        {
            THEN("begin() returns element")
            {
                REQUIRE(*v.begin() == value);
            }
            THEN("end() returns element")
            {
                REQUIRE(*(v.end() - 1) == value);
            }
            THEN("the get_allocator() returns original instance")
            {
                REQUIRE(allocator == v.get_allocator());
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Constructor allocator passing - NoAllocatorType",
                        "[phase3][vector][constructor]",
                        pw::test::TestTypeListNoAllocator)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    using size_type  = typename Vector::size_type;

    GIVEN("vector constructed with NoAllocatorType elements")
    {
        WHEN("constructing with count constructor")
        {
            auto                counter_before = value_type::getCounter();
            constexpr size_type count          = 3;
            Vector              v(count);
            auto                counter_after = value_type::getCounter();
            auto                diff          = counter_after - counter_before;

            THEN("allocator is not passed to value_type constructor")
            {
                REQUIRE(v.size() == count);
                REQUIRE(diff.getNoAllocator() == static_cast<int>(count));
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }

        WHEN("constructing with count and value constructor")
        {
            auto                counter_before = value_type::getCounter();
            constexpr size_type count          = 2;
            constexpr int       initial_value  = 42;
            Vector              v(count, value_type(initial_value));
            auto                counter_after = value_type::getCounter();
            auto                diff          = counter_after - counter_before;

            THEN("allocator is not passed to value_type constructor")
            {
                REQUIRE(v.size() == count);
                REQUIRE(diff.getNoAllocator() >= static_cast<int>(count));
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Constructor allocator passing - AllocatorFirstType",
                        "[phase3][vector][constructor]",
                        pw::test::TestTypeListAllocatorFirst)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    using size_type  = typename Vector::size_type;

    GIVEN("vector constructed with AllocatorFirstType elements")
    {
        WHEN("constructing with count constructor")
        {
            auto                counter_before = value_type::getCounter();
            constexpr size_type count          = 3;
            Vector              v(count);
            auto                counter_after = value_type::getCounter();
            auto                diff          = counter_after - counter_before;

            THEN("allocator is passed as first argument to value_type constructor")
            {
                REQUIRE(v.size() == count);
                REQUIRE(diff.getAllocatorFirst() == static_cast<int>(count));
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }

        WHEN("constructing with count and value constructor")
        {
            auto                counter_before = value_type::getCounter();
            constexpr size_type count          = 2;
            constexpr int       initial_value  = 42;
            Vector              v(count,
                     value_type(std::allocator_arg, typename value_type::allocator_type {}, initial_value));
            auto                counter_after = value_type::getCounter();
            auto                diff          = counter_after - counter_before;

            THEN("allocator is passed as first argument to value_type constructor")
            {
                REQUIRE(v.size() == count);
                REQUIRE(diff.getAllocatorFirst() >= static_cast<int>(count));
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Constructor allocator passing - AllocatorLastType",
                        "[phase3][vector][constructor]",
                        pw::test::TestTypeListAllocatorLast)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    using size_type  = typename Vector::size_type;

    GIVEN("vector constructed with AllocatorLastType elements")
    {
        WHEN("constructing with count constructor")
        {
            auto                counter_before = value_type::getCounter();
            constexpr size_type count          = 3;
            Vector              v(count);
            auto                counter_after = value_type::getCounter();
            auto                diff          = counter_after - counter_before;

            THEN("allocator is passed as last argument to value_type constructor")
            {
                REQUIRE(v.size() == count);
                REQUIRE(diff.getAllocatorLast() == static_cast<int>(count));
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }

        WHEN("constructing with count and value constructor")
        {
            auto                counter_before = value_type::getCounter();
            constexpr size_type count          = 2;
            constexpr int       initial_value  = 42;
            Vector              v(count, value_type(initial_value, typename value_type::allocator_type {}));
            auto                counter_after = value_type::getCounter();
            auto                diff          = counter_after - counter_before;

            THEN("allocator is passed as last argument to value_type constructor")
            {
                REQUIRE(v.size() == count);
                REQUIRE(diff.getAllocatorLast() >= static_cast<int>(count));
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Constructor allocator passing - AllocatorOnlyType",
                        "[phase3][vector][constructor]",
                        pw::test::TestTypeListAllocatorOnly)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;
    using size_type  = typename Vector::size_type;

    GIVEN("vector constructed with AllocatorOnlyType elements")
    {
        REQUIRE(std::is_constructible_v<value_type>);
        REQUIRE(std::uses_allocator_v<value_type, typename Vector::allocator_type>);
        // WHEN("constructing with count constructor")
        // {
        //     auto                counter_before = value_type::getCounter();
        //     constexpr size_type count          = 3;
        //     Vector              v; //(count);
        //     auto                counter_after = value_type::getCounter();
        //     auto                diff          = counter_after - counter_before;
        //
        //     THEN("allocator is passed as first argument to value_type constructor")
        //     {
        //         REQUIRE(v.size() == count);
        //         REQUIRE(diff.getAllocatorFirst() == static_cast<int>(count));
        //         REQUIRE(diff.getNoAllocator() == 0);
        //         REQUIRE(diff.getAllocatorLast() == 0);
        //         REQUIRE(diff.getAllocatorOnly() == static_cast<int>(count));
        //     }
        // }
    }
}