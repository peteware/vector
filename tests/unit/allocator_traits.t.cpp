#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>

#include <test_allocator_base.h>
#include <test_emplacemoveconstructible.h>

#include <catch2/catch_test_macros.hpp>
#include <limits>

namespace pw::test {

// Test allocator that provides select_on_container_copy_construction
template<class Type>
struct allocator_with_select : public allocator_base<Type>
{
    using base = allocator_base<Type>;

    explicit allocator_with_select(int instance = 1)
        : base(instance)
    {
    }

    // Custom select_on_container_copy_construction that returns instance + 100
    allocator_with_select select_on_container_copy_construction() const
    {
        return allocator_with_select(base::m_instance + 100);
    }
};

// Test allocator that does NOT provide select_on_container_copy_construction
template<class Type>
struct allocator_without_select : public allocator_base<Type>
{
    using base = allocator_base<Type>;

    explicit allocator_without_select(int instance = 1)
        : base(instance)
    {
    }

    // Intentionally no select_on_container_copy_construction method
};

// Test allocator that provides max_size
template<class Type>
struct allocator_with_max_size : public allocator_base<Type>
{
    using base      = allocator_base<Type>;
    using size_type = typename base::size_type;

    explicit allocator_with_max_size(int instance = 1)
        : base(instance)
    {
    }

    size_type max_size() const { return 12345; }
};

} // namespace pw::test

SCENARIO("Allocator traits", "[allocator_traits]")
{
    GIVEN("An allocator of int")
    {
        pw::allocator<int> alloc;
        int                x;

        WHEN("construct that int")
        {
            pw::allocator_traits<pw::allocator<int>>::construct(alloc, &x, 3);
            THEN("value is set")
            {
                REQUIRE(3 == x);
            }
        }
    }
    GIVEN("An allocator of EmplaceMoveConstructible")
    {
        pw::allocator<pw::test::EmplaceMoveConstructible> alloc;
        pw::test::EmplaceMoveConstructible                x(4, 5);

        WHEN("construct")
        {
            pw::allocator_traits<pw::allocator<pw::test::EmplaceMoveConstructible>>::construct(
                alloc, &x, 6, 8);
            THEN("value is set")
            {
                REQUIRE(6 == x.value());
                REQUIRE(8 == x.value2());
            }
        }
    }
}

SCENARIO("select_on_container_copy_construction", "[allocator_traits][select_on_container_copy_construction]")
{
    GIVEN("An allocator that provides select_on_container_copy_construction")
    {
        pw::test::allocator_with_select<int> alloc(42);

        WHEN("select_on_container_copy_construction is called")
        {
            auto result = pw::allocator_traits<
                pw::test::allocator_with_select<int>>::select_on_container_copy_construction(alloc);

            THEN("the allocator's custom method is used")
            {
                REQUIRE(result.m_instance == 142); // 42 + 100
                REQUIRE(alloc.m_instance == 42);   // original unchanged
            }
        }
    }

    GIVEN("An allocator that does NOT provide select_on_container_copy_construction")
    {
        pw::test::allocator_without_select<int> alloc(55);

        WHEN("select_on_container_copy_construction is called")
        {
            auto result = pw::allocator_traits<
                pw::test::allocator_without_select<int>>::select_on_container_copy_construction(alloc);

            THEN("a copy of the allocator is returned")
            {
                REQUIRE(result.m_instance == 55); // same as original
                REQUIRE(alloc.m_instance == 55);  // original unchanged
            }
        }
    }

    GIVEN("The standard pw::allocator")
    {
        pw::allocator<int> alloc;

        WHEN("select_on_container_copy_construction is called")
        {
            auto result = pw::allocator_traits<pw::allocator<int>>::select_on_container_copy_construction(
                alloc);

            THEN("a copy is returned (standard allocator has no custom method)")
            {
                // Both allocators should be equivalent (standard allocator is stateless)
                REQUIRE(result == alloc);
            }
        }
    }

    GIVEN("Different allocator instances with select_on_container_copy_construction")
    {
        pw::test::allocator_with_select<double> alloc1(10);
        pw::test::allocator_with_select<double> alloc2(20);

        WHEN("select_on_container_copy_construction is called on both")
        {
            auto result1 = pw::allocator_traits<
                pw::test::allocator_with_select<double>>::select_on_container_copy_construction(alloc1);
            auto result2 = pw::allocator_traits<
                pw::test::allocator_with_select<double>>::select_on_container_copy_construction(alloc2);

            THEN("each gets the correct transformed instance")
            {
                REQUIRE(result1.m_instance == 110); // 10 + 100
                REQUIRE(result2.m_instance == 120); // 20 + 100
                REQUIRE(result1.m_instance != result2.m_instance);
            }
        }
    }

    GIVEN("Different value types with the same allocator template")
    {
        pw::test::allocator_with_select<int>    int_alloc(30);
        pw::test::allocator_with_select<double> double_alloc(30);

        WHEN("select_on_container_copy_construction is called on both")
        {
            auto int_result = pw::allocator_traits<
                pw::test::allocator_with_select<int>>::select_on_container_copy_construction(int_alloc);
            auto double_result = pw::allocator_traits<
                pw::test::allocator_with_select<double>>::select_on_container_copy_construction(double_alloc);

            THEN("both work correctly with their respective types")
            {
                REQUIRE(int_result.m_instance == 130);    // 30 + 100
                REQUIRE(double_result.m_instance == 130); // 30 + 100
            }
        }
    }
}

SCENARIO("max_size", "[allocator_traits][max_size]")
{
    GIVEN("An allocator that provides max_size()")
    {
        pw::test::allocator_with_max_size<int> alloc;

        WHEN("max_size is called on traits")
        {
            auto s = pw::allocator_traits<pw::test::allocator_with_max_size<int>>::max_size(alloc);
            THEN("it is the allocator's max_size")
            {
                REQUIRE(s == 12345);
            }
        }
    }
    GIVEN("An allocator that does not provide max_size()")
    {
        pw::test::allocator_base<int> alloc;
        using traits = pw::allocator_traits<pw::test::allocator_base<int>>;

        WHEN("max_size is called on traits")
        {
            auto s = traits::max_size(alloc);
            THEN("it is the default")
            {
                REQUIRE(s == std::numeric_limits<traits::size_type>::max() / sizeof(int));
            }
        }
    }
}
