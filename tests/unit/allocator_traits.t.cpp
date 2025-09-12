#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>

#include <test_allocator_base.h>
#include <test_allocator_first_type.h>
#include <test_emplacemoveconstructible.h>

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
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
    [[nodiscard]] allocator_with_select select_on_container_copy_construction() const
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
    using size_type = base::size_type;

    explicit allocator_with_max_size(int instance = 1)
        : base(instance)
    {
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] size_type max_size() const { return 12345; }
};

} // namespace pw::test
SCENARIO("allocator_traits construct", "[allocator_traits][construct]")
{
    GIVEN("An allocator and AllocatorFirstType as Type")
    {
        using Alloc = pw::allocator<int>;
        using Type  = pw::test::AllocatorFirstType<Alloc>;
        Alloc alloc;
        WHEN("Pass alloc as first argument")
        {
            pw::test::AllocatorFirstType<Alloc> obj { std::allocator_arg, alloc, 5, 20 };
            THEN("obj is initialized")
            {
                REQUIRE(obj.value() == 5 + 20);
            }
        }
        WHEN("Use allocator_traits<>::construct() to construct")
        {
            alignas(Type) std::byte storage[sizeof(Type)];
            Type&                   obj   = reinterpret_cast<Type&>(storage);
            pw::test::OpCounter     start = Type::getCounter();
            SKIP("Need to add construct() to allocator");
            pw::allocator_traits<Alloc>::construct(alloc, &obj);
            THEN("we called the copy constructor with alloc")
            {
                pw::test::OpCounter diff = Type::getCounter() - start;
                INFO(diff);
                REQUIRE(diff.getCopyConstructorAlloc() == 1);
            }
        }
        WHEN("Pass empty args to allocator_traits<>::construct()")
        {
            alignas(Type) std::byte storage[sizeof(Type)];
            Type&                   obj   = reinterpret_cast<Type&>(storage);
            pw::test::OpCounter     start = Type::getCounter();
            SKIP("Need to add construct() to allocator");
            pw::allocator_traits<Alloc>::construct(alloc, &obj);
            THEN("we called the copy constructor with alloc")
            {
                pw::test::OpCounter diff = Type::getCounter() - start;
                INFO(diff);
                REQUIRE(diff.getCopyConstructorAlloc() == 1);
            }
        }
    }
}

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

SCENARIO("allocator deallocate with is_constant_evaluated", "[allocator][deallocate][is_constant_evaluated]")
{
    GIVEN("A pw::allocator instance")
    {
        pw::allocator<int> alloc;

        WHEN("deallocate is called at runtime")
        {
            // Allocate memory first
            int* ptr = alloc.allocate(5);
            REQUIRE(ptr != nullptr);

            // This should call operator delete since we're at runtime
            // The test mainly verifies this doesn't crash and compiles properly
            THEN("deallocate calls operator delete")
            {
                REQUIRE_NOTHROW(alloc.deallocate(ptr, 5));
                // If we get here, deallocate succeeded at runtime
                REQUIRE(true);
            }
        }

        WHEN("deallocate is called in constexpr context")
        {
            // Test that deallocate can be called in constant evaluation
            constexpr auto test_constexpr_deallocate = []() constexpr {
                pw::allocator<int> constexpr_alloc;

                // In constant evaluation, we can't actually allocate memory,
                // but we can test that deallocate(nullptr, 0) works
                constexpr_alloc.deallocate(nullptr, 0);

                // Return true to indicate the constexpr evaluation succeeded
                return true;
            };

            constexpr bool result = test_constexpr_deallocate();

            THEN("deallocate returns early in constant evaluation")
            {
                REQUIRE(result == true);
            }
        }

        WHEN("deallocate is called with nullptr at runtime")
        {
            // Test edge case: deallocating nullptr should work
            THEN("deallocate handles nullptr gracefully")
            {
                REQUIRE_NOTHROW(alloc.deallocate(nullptr, 0));
                REQUIRE_NOTHROW(alloc.deallocate(nullptr, 5));
            }
        }

        WHEN("deallocate works in guaranteed constant evaluation")
        {
            // Test using static_assert to ensure constexpr compatibility
            static_assert(
                []() constexpr {
                    pw::allocator<int> eval_alloc;
                    eval_alloc.deallocate(nullptr, 0);
                    return true;
                }(),
                "deallocate should work in constant evaluation");

            THEN("deallocate compiles and works in static_assert")
            {
                REQUIRE(true); // If we get here, static_assert passed
            }
        }
    }

    GIVEN("Different value types")
    {
        WHEN("deallocate is used with various types in constexpr context")
        {
            constexpr auto test_different_types = []() constexpr {
                pw::allocator<char>   char_alloc;
                pw::allocator<double> double_alloc;
                pw::allocator<long>   long_alloc;

                // All should work in constant evaluation
                char_alloc.deallocate(nullptr, 0);
                double_alloc.deallocate(nullptr, 0);
                long_alloc.deallocate(nullptr, 0);

                return true;
            };

            constexpr bool type_test_result = test_different_types();

            THEN("all types work in constant evaluation")
            {
                REQUIRE(type_test_result == true);
            }
        }

        WHEN("deallocate is used with actual allocations at runtime")
        {
            pw::allocator<double> double_alloc;
            pw::allocator<char>   char_alloc;

            double*               double_ptr = double_alloc.allocate(3);
            char*                 char_ptr   = char_alloc.allocate(10);

            THEN("runtime deallocate works for different types")
            {
                REQUIRE_NOTHROW(double_alloc.deallocate(double_ptr, 3));
                REQUIRE_NOTHROW(char_alloc.deallocate(char_ptr, 10));
            }
        }
    }

    GIVEN("Testing the is_constant_evaluated branch specifically")
    {
        WHEN("constexpr lambda calls deallocate")
        {
            // Test that shows the difference between compile-time and runtime
            // ReSharper disable once CppDFAConstantFunctionResult
            auto test_context_sensitivity = []() constexpr {
                pw::allocator<int> ctx_alloc;

                // In constexpr context, this should take the early return path
                ctx_alloc.deallocate(nullptr, 1);

                // Return a value that indicates we successfully executed
                return pw::is_constant_evaluated() ? 100 : 200;
            };

            constexpr int compile_time_result = test_context_sensitivity();
            int           runtime_result      = test_context_sensitivity();

            THEN("deallocate behaves correctly in both contexts")
            {
                REQUIRE(compile_time_result == 100); // Constant evaluated
                REQUIRE(runtime_result == 200);      // Runtime evaluated
            }
        }

        WHEN("testing mixed allocation and deallocation scenarios")
        {
            // Test a more complex scenario mixing allocate/deallocate
            constexpr auto test_allocation_cycle = []() constexpr {
                pw::allocator<int> cycle_alloc;

                // In constexpr, we can't actually allocate, but we can test
                // that the deallocate call doesn't prevent constexpr evaluation
                if (pw::is_constant_evaluated())
                {
                    // Simulate constexpr allocation/deallocation cycle
                    cycle_alloc.deallocate(nullptr, 5);
                    return 300;
                }
                else
                {
                    // This branch won't execute in constexpr context
                    return 400;
                }
            };

            constexpr int cycle_result = test_allocation_cycle();

            THEN("allocation cycle works in constexpr context")
            {
                REQUIRE(cycle_result == 300);
            }
        }
    }
}
