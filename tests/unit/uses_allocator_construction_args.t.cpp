#include <pw/memory>
#include <test_allocator_first_type.h>
#include <test_allocator_last_type.h>
#include <test_allocator_only_type.h>
#include <test_no_allocator_type.h>

#include <catch2/catch_test_macros.hpp>
#include <memory>

using TestAllocator = pw::allocator<int>;

SCENARIO("uses_allocator_construction_args for non-allocator-aware types",
         "[memory][uses_allocator_construction_args]")
{
    GIVEN("A type that doesn't use allocators")
    {
        using Type = pw::test::NoAllocatorType;
        TestAllocator alloc;

        WHEN("called with no additional arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns args without allocator")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 0);
            }
        }

        WHEN("called with value argument")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42);

            THEN("returns args with value")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 1);
            }
        }

        WHEN("called with multiple arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42, 10);

            THEN("returns args with all arguments")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 2);
            }
        }
    }
}

SCENARIO("uses_allocator_construction_args for allocator-first types",
         "[memory][uses_allocator_construction_args]")
{
    GIVEN("A type that uses allocators with allocator_arg_t in first position")
    {
        using Type = pw::test::AllocatorFirstType<TestAllocator>;
        TestAllocator alloc;

        WHEN("called with no additional arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns (allocator_arg, alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 2);
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, std::allocator_arg_t>);
            }
        }

        WHEN("called with value argument")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42);

            THEN("returns (allocator_arg, alloc, value)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 3);
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, std::allocator_arg_t>);
            }
        }

        WHEN("default constructor (with alloc argument)")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns (alloc)")
            {
                INFO(typeid(decltype(args)).name());
                INFO((std::is_constructible_v<Type, TestAllocator const&>));
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, decltype(alloc)>);
                REQUIRE(std::tuple_size_v<decltype(args)> == 1);
                REQUIRE(false);
            }
        }

        WHEN("called with multiple arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42, 10);

            THEN("returns (allocator_arg, alloc, value, extra)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 4);
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, std::allocator_arg_t>);
            }
        }
    }
}

SCENARIO("uses_allocator_construction_args for allocator-last types",
         "[memory][uses_allocator_construction_args]")
{
    GIVEN("A type that uses allocators with allocator in last position")
    {
        using Type = pw::test::AllocatorLastType<TestAllocator>;
        TestAllocator alloc;

        WHEN("called with no additional arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns (alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 1);
            }
        }

        WHEN("called with value argument")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42);

            THEN("returns (value, alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 2);
            }
        }

        WHEN("called with multiple arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42, 10);

            THEN("returns (value, extra, alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 3);
            }
        }
    }
}

SCENARIO("uses_allocator_construction_args for allocator-only types",
         "[memory][uses_allocator_construction_args]")
{
    GIVEN("A type that only accepts an allocator")
    {
        using Type = pw::test::AllocatorOnlyType;
        TestAllocator alloc;

        WHEN("called with no additional arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns (allocator_arg, alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 2);
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, std::allocator_arg_t>);
            }
        }
    }
}

SCENARIO("uses_allocator_construction_args behavior verification with OpTracker",
         "[memory][uses_allocator_construction_args]")
{
    GIVEN("Test types with OpTracker")
    {
        TestAllocator alloc;

        WHEN("constructing NoAllocatorType with arguments")
        {
            auto counter_before = pw::test::NoAllocatorType::getCounter();
            auto args = pw::uses_allocator_construction_args<pw::test::NoAllocatorType>(alloc, 42, 10);
            auto counter_after = pw::test::NoAllocatorType::getCounter();
            auto diff          = counter_after - counter_before;

            THEN("no allocator style operations are tracked yet")
            {
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }

        WHEN("constructing AllocatorFirstType with arguments")
        {
            using Type          = pw::test::AllocatorFirstType<TestAllocator>;
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc, 42, 10);
            auto counter_after  = Type::getCounter();
            auto diff           = counter_after - counter_before;

            THEN("no allocator style operations are tracked yet")
            {
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }
    }
}