#include <pw/algorithm>
#include <pw/vector>

#include <initializer_list>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

// Phase 1 test type list - using int with pw::vector and std::vector for comparison
using Phase1TestTypeList = std::tuple<pw::vector<int>, std::vector<int>>;

TEMPLATE_LIST_TEST_CASE("Initializer list constructor",
                        "[phase1][vector][constructor][init-list]",
                        Phase1TestTypeList)
{
    using Vector = TestType;
    GIVEN("An initializer list")
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
}

TEMPLATE_LIST_TEST_CASE("Default constructor", "[phase1][vector][constructor][default]", Phase1TestTypeList)
{
    using Vector = TestType;
    GIVEN("A default constructed vector")
    {
        WHEN("default constructed")
        {
            Vector v;

            THEN("the vector is empty")
            {
                Vector expected {};
                REQUIRE(v.empty() == true);
                REQUIRE(v.begin() == v.end());
                REQUIRE(v == expected);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Count constructor", "[phase1][vector][constructor][count]", Phase1TestTypeList)
{
    using Vector = TestType;
    GIVEN("A vector constructed with count only")
    {
        WHEN("constructed with size 0")
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
        WHEN("constructed with size 5")
        {
            Vector v(5);

            THEN("the vector has 5 default-constructed elements")
            {
                Vector expected { 0, 0, 0, 0, 0 };
                REQUIRE(v.size() == 5);
                REQUIRE(v.empty() == false);
                REQUIRE(v.capacity() >= 5);
                REQUIRE(v == expected);
            }
        }
        WHEN("constructed with size 1")
        {
            Vector v(1);

            THEN("the vector has 1 default-constructed element")
            {
                Vector expected { 0 };
                REQUIRE(v.size() == 1);
                REQUIRE(v.empty() == false);
                REQUIRE(v.capacity() >= 1);
                REQUIRE(v == expected);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Count-value constructor",
                        "[phase1][vector][constructor][count-value]",
                        Phase1TestTypeList)
{
    using Vector = TestType;
    GIVEN("A vector constructed with count and value")
    {
        WHEN("constructed with size 0 and value 42")
        {
            Vector v(static_cast<typename Vector::size_type>(0), 42);

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
            Vector v(static_cast<typename Vector::size_type>(3), 42);

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
            Vector v(static_cast<typename Vector::size_type>(1), -7);

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
}

TEMPLATE_LIST_TEST_CASE("Copy constructor", "[phase1][vector][constructor][copy]", Phase1TestTypeList)
{
    using Vector = TestType;
    GIVEN("A source vector")
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
            Vector copy(source);

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
            Vector copy(source);

            THEN("the copy has identical content")
            {
                Vector expected { 1, 2, 3, 4 };
                REQUIRE(copy.size() == 4);
                REQUIRE(copy == expected);
                REQUIRE(copy == source);
            }
        }
        WHEN("copy constructor from larger vector")
        {
            Vector source { 10, 20, 30, 40, 50 };
            Vector copy(source);

            THEN("the copy has the source content")
            {
                Vector expected { 10, 20, 30, 40, 50 };
                REQUIRE(copy == expected);
                REQUIRE(copy == source);
            }
        }
    }
}

TEMPLATE_LIST_TEST_CASE("Move constructor", "[phase1][vector][constructor][move]", Phase1TestTypeList)
{
    using Vector = TestType;
    GIVEN("A source vector for moving")
    {
        WHEN("moving an empty vector")
        {
            Vector source {};
            Vector original_copy(source); // Keep a copy for comparison
            Vector moved(std::move(source));

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
}

TEMPLATE_LIST_TEST_CASE("Iterator range constructor",
                        "[phase1][vector][constructor][range]",
                        Phase1TestTypeList)
{
    using Vector = TestType;

    GIVEN("A source container for iterator range")
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
    }
}