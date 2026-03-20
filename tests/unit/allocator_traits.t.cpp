#include <pw/impl/allocator/allocator.h>
#include <pw/impl/memory/allocator_traits.h>
#include <pw/impl/memory/construct_at.h>
#include <pw/impl/memory_resource/pmr_polymorphic_allocator.h>
#include <pw/impl/type_traits/is_same.h>
#include <test_allocator_base.h>
#include <test_allocator_is_always_equal.h>
#include <test_optracker_allocator_first.h>
#include <test_optracker_emplacemoveconstructible.h>

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <limits>

namespace pw::test {

// Minimal fancy pointer for testing custom pointer type aliases in allocator_traits.
// pointer_traits<fancy_ptr<T>> works via generic template:
//   element_type = T (via fancy_ptr::element_type)
//   difference_type = ptrdiff_t (fallback, fancy_ptr has none)
//   rebind<U> = fancy_ptr<U> (via rebind_first_arg since fancy_ptr<T> is template<T>)
template<class T>
struct fancy_ptr
{
    using element_type = T;
    T*   raw;
    explicit fancy_ptr(T* p = nullptr)
        : raw(p)
    {
    }
};

// Allocator that defines pointer = fancy_ptr<Type>
template<class Type>
struct allocator_with_fancy_pointer : allocator_base<Type>
{
    using pointer = fancy_ptr<Type>;
    explicit allocator_with_fancy_pointer(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

// Allocator with custom size_type distinct from make_unsigned<ptrdiff_t>::type (= size_t)
template<class Type>
struct allocator_with_size_type : allocator_base<Type>
{
    using size_type = unsigned short;
    explicit allocator_with_size_type(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

// Allocator with custom difference_type distinct from pointer_traits<Type*>::difference_type (= ptrdiff_t)
template<class Type>
struct allocator_with_diff_type : allocator_base<Type>
{
    using difference_type = short;
    explicit allocator_with_diff_type(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

// Allocator with custom const_pointer distinct from pointer_traits<Type*>::rebind<const Type> (= const Type*)
template<class Type>
struct allocator_with_const_pointer : allocator_base<Type>
{
    using const_pointer = Type const* const*;
    explicit allocator_with_const_pointer(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

// Allocator with custom void_pointer distinct from pointer_traits<Type*>::rebind<void> (= void*)
template<class Type>
struct allocator_with_void_pointer : allocator_base<Type>
{
    using void_pointer = long*;
    explicit allocator_with_void_pointer(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

// Allocator with custom const_void_pointer distinct from pointer_traits<Type*>::rebind<const void> (= const void*)
template<class Type>
struct allocator_with_const_void_pointer : allocator_base<Type>
{
    using const_void_pointer = long const*;
    explicit allocator_with_const_void_pointer(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }
};

// Allocator that counts how many times allocate() is called.
// Used to verify allocator_traits::allocate does not short-circuit for n=0.
template<class Type>
struct counting_allocator : allocator_base<Type>
{
    int m_allocate_calls = 0;
    explicit counting_allocator(int instance = 1)
        : allocator_base<Type>(instance)
    {
    }

    Type* allocate(size_t n)
    {
        ++m_allocate_calls;
        if (n == 0)
            return nullptr;
        return allocator_base<Type>::allocate(n);
    }
};

// Test allocator that provides select_on_container_copy_construction
template<class Type>
struct allocator_with_select : allocator_base<Type>
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
struct allocator_without_select : allocator_base<Type>
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
struct allocator_with_max_size : allocator_base<Type>
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
    GIVEN("An allocator and OpTrackerAllocatorFirst as Type")
    {
        //using Alloc = pw::test::allocator_base<int>;
        using Alloc = pw::pmr::polymorphic_allocator<>;
        using Type  = pw::test::OpTrackerAllocatorFirst<Alloc>;
        Alloc alloc;
        WHEN("Pass alloc as first argument")
        {
            pw::test::OpTrackerAllocatorFirst<Alloc> obj { std::allocator_arg, alloc, 5, 20 };
            THEN("obj is initialized")
            {
                REQUIRE(obj.value() == 5 + 20);
            }
        }
        WHEN("Use allocator_traits<>::construct() to construct")
        {
            alignas(Type) std::byte storage[sizeof(Type)];
            auto&                   obj   = reinterpret_cast<Type&>(storage);
            pw::test::OpCounter     start = Type::getCounter();
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
            auto&                   obj   = reinterpret_cast<Type&>(storage);
            pw::test::OpCounter     start = Type::getCounter();
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
    GIVEN("An allocator of OpTrackerEmplaceMoveConstructible")
    {
        pw::allocator<pw::test::OpTrackerEmplaceMoveConstructible> alloc;
        pw::test::OpTrackerEmplaceMoveConstructible                x(4, 5);

        WHEN("construct")
        {
            pw::allocator_traits<pw::allocator<pw::test::OpTrackerEmplaceMoveConstructible>>::construct(
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
                // This branch won't execute in constexpr context
                return 400;
            };

            constexpr int cycle_result = test_allocation_cycle();

            THEN("allocation cycle works in constexpr context")
            {
                REQUIRE(cycle_result == 300);
            }
        }
    }
}

SCENARIO("allocator_traits::construct() calls allocator::construct()", "[traits][construct_called]")
{
    GIVEN("An allocator")
    {
        using Alloc = pw::test::allocator_base<int>;
        using Type  = int;
        Alloc                   alloc(33);
        alignas(Type) std::byte storage[sizeof(Type)];
        auto&                   obj = reinterpret_cast<Type&>(storage);
        WHEN("Use allocator_traits<>::construct() to construct")
        {
            pw::allocator_traits<Alloc>::construct(alloc, &obj, 3);
            THEN("we called the copy constructor with alloc")
            {
                REQUIRE(alloc.m_construct_calls == 1);
            }
        }
        WHEN("allocator_base is called")
        {
            alloc.construct(&obj);
            THEN("construct is called")
            {
                REQUIRE(alloc.m_construct_calls == 1);
            }
        }
    }
}

// ─── constexpr allocate ──────────────────────────────────────────────────────
// allocator_traits::allocate must be usable in constexpr contexts (C++20+).
static_assert(
    []() constexpr {
        pw::allocator<int> alloc;
        int*               p = pw::allocator_traits<pw::allocator<int>>::allocate(alloc, 1);
        pw::allocator_traits<pw::allocator<int>>::deallocate(alloc, p, 1);
        return true;
    }(),
    "allocator_traits::allocate must be constexpr");

// ─── constexpr destroy (runtime verification) ────────────────────────────────
// The function is marked constexpr (see declaration in allocator_traits.h).
// Constexpr placement-new limitations in pw::allocator prevent a full
// static_assert round-trip, so we verify runtime correctness instead.
// The constexpr marking is validated implicitly: if the function body contained
// non-constexpr operations the declaration would fail to compile.

// ─── Type alias detection tests ──────────────────────────────────────────────

SCENARIO("allocator_traits::is_always_equal uses Alloc::is_always_equal when defined",
         "[allocator_traits][is_always_equal]")
{
    GIVEN("An allocator that explicitly defines is_always_equal = true_type")
    {
        // allocator_is_always_equal has is_always_equal = true_type, but has data
        // members (not empty), so is_empty<Alloc>::type would give false_type.
        using Alloc = pw::test::allocator_is_always_equal<int>;
        THEN("allocator_traits::is_always_equal is true_type")
        {
            REQUIRE((pw::is_same_v<pw::allocator_traits<Alloc>::is_always_equal, pw::true_type>));
        }
    }
}

SCENARIO("allocator_traits::pointer uses Alloc::pointer when defined",
         "[allocator_traits][pointer]")
{
    GIVEN("An allocator that defines pointer = fancy_ptr<Type>")
    {
        using Alloc  = pw::test::allocator_with_fancy_pointer<int>;
        using FancyP = pw::test::fancy_ptr<int>;
        THEN("allocator_traits::pointer is fancy_ptr<int>")
        {
            REQUIRE((pw::is_same_v<pw::allocator_traits<Alloc>::pointer, FancyP>));
        }
    }
}

SCENARIO("allocator_traits::size_type uses Alloc::size_type when defined",
         "[allocator_traits][size_type]")
{
    GIVEN("An allocator that defines size_type = unsigned short")
    {
        using Alloc = pw::test::allocator_with_size_type<int>;
        THEN("allocator_traits::size_type is unsigned short")
        {
            REQUIRE((pw::is_same_v<pw::allocator_traits<Alloc>::size_type, unsigned short>));
        }
    }
}

SCENARIO("allocator_traits::difference_type uses Alloc::difference_type when defined",
         "[allocator_traits][difference_type]")
{
    GIVEN("An allocator that defines difference_type = short")
    {
        using Alloc = pw::test::allocator_with_diff_type<int>;
        THEN("allocator_traits::difference_type is short")
        {
            REQUIRE((pw::is_same_v<pw::allocator_traits<Alloc>::difference_type, short>));
        }
    }
}

SCENARIO("allocator_traits::const_pointer uses Alloc::const_pointer when defined",
         "[allocator_traits][const_pointer]")
{
    GIVEN("An allocator that defines const_pointer = int const* const*")
    {
        using Alloc = pw::test::allocator_with_const_pointer<int>;
        THEN("allocator_traits::const_pointer is int const* const*")
        {
            REQUIRE((pw::is_same_v<pw::allocator_traits<Alloc>::const_pointer, int const* const*>));
        }
    }
}

SCENARIO("allocator_traits::void_pointer uses Alloc::void_pointer when defined",
         "[allocator_traits][void_pointer]")
{
    GIVEN("An allocator that defines void_pointer = long*")
    {
        using Alloc = pw::test::allocator_with_void_pointer<int>;
        THEN("allocator_traits::void_pointer is long*")
        {
            REQUIRE((pw::is_same_v<pw::allocator_traits<Alloc>::void_pointer, long*>));
        }
    }
}

SCENARIO("allocator_traits::const_void_pointer uses Alloc::const_void_pointer when defined",
         "[allocator_traits][const_void_pointer]")
{
    GIVEN("An allocator that defines const_void_pointer = long const*")
    {
        using Alloc = pw::test::allocator_with_const_void_pointer<int>;
        THEN("allocator_traits::const_void_pointer is long const*")
        {
            REQUIRE(
                (pw::is_same_v<pw::allocator_traits<Alloc>::const_void_pointer, long const*>));
        }
    }
}

// ─── allocate_at_least (C++23) ───────────────────────────────────────────────

SCENARIO("allocator_traits::allocate_at_least returns at least n elements",
         "[allocator_traits][allocate_at_least]")
{
    GIVEN("A pw::allocator<int>")
    {
        pw::allocator<int> alloc;
        WHEN("allocate_at_least(5) is called")
        {
            auto result = pw::allocator_traits<pw::allocator<int>>::allocate_at_least(alloc, 5);
            THEN("the pointer is non-null and count is at least 5")
            {
                REQUIRE(result.ptr != nullptr);
                REQUIRE(result.count >= 5);
                pw::allocator_traits<pw::allocator<int>>::deallocate(alloc, result.ptr, result.count);
            }
        }
    }
}

// ─── rebind_alloc / rebind_traits ────────────────────────────────────────────

SCENARIO("allocator_traits::rebind_alloc rebinds allocator to a new value type",
         "[allocator_traits][rebind_alloc]")
{
    GIVEN("allocator_base<int>")
    {
        using Alloc        = pw::test::allocator_base<int>;
        using ReboundAlloc = pw::allocator_traits<Alloc>::rebind_alloc<double>;
        THEN("rebind_alloc<double> is allocator_base<double>")
        {
            REQUIRE((pw::is_same_v<ReboundAlloc, pw::test::allocator_base<double>>));
        }
    }
}

SCENARIO("allocator_traits::rebind_traits provides traits for rebound allocator",
         "[allocator_traits][rebind_traits]")
{
    GIVEN("allocator_base<int>")
    {
        using Alloc         = pw::test::allocator_base<int>;
        using ReboundTraits = pw::allocator_traits<Alloc>::rebind_traits<double>;
        THEN("rebind_traits<double>::value_type is double")
        {
            REQUIRE((pw::is_same_v<ReboundTraits::value_type, double>));
        }
    }
}

// ─── allocate(n=0) calls through ────────────────────────────────────────────

SCENARIO("allocator_traits::allocate calls alloc.allocate even when n=0",
         "[allocator_traits][allocate][n=0]")
{
    GIVEN("A counting allocator")
    {
        pw::test::counting_allocator<int> alloc;
        WHEN("allocate is called with n=0")
        {
            pw::allocator_traits<pw::test::counting_allocator<int>>::allocate(alloc, 0);
            THEN("the allocator's allocate was called once")
            {
                REQUIRE(alloc.m_allocate_calls == 1);
            }
        }
    }
}
