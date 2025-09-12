#include <pw/impl/is_same.h>
#include <pw/memory>
#include <test_optracker_allocator_first.h>
#include <test_optracker_allocator_last.h>
#include <test_optracker_allocator_none.h>
#include <test_optracker_allocator_only.h>

#include <catch2/catch_test_macros.hpp>
#include <memory>

using TestAllocator = pw::allocator<int>;

SCENARIO("uses_allocator_construction_args for non-allocator-aware types",
         "[memory][uses_allocator_construction_args]")
{
    GIVEN("A type that doesn't use allocators")
    {
        using Type = pw::test::OpTrackerAllocatorNone;
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
        using Type = pw::test::OpTrackerAllocatorFirst<TestAllocator>;
        Type::allocator_type alloc;

        WHEN("called with no additional arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns (allocator_arg, alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 1);
                using Arg0Type = std::tuple_element_t<0, decltype(args)>;
                INFO("tuple0 = " << typeid(std::tuple_element_t<0, decltype(args)>).name()
                                 << " Arg0Type = " << typeid(Arg0Type).name()
                                 << " Type::allocator_type = " << typeid(Type::allocator_type).name());
                REQUIRE(typeid(std::tuple_element_t<0, decltype(args)>) == typeid(Type::allocator_type));
                REQUIRE(pw::is_same_v<std::tuple_element_t<0, decltype(args)>, Type::allocator_type const&>);
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
                INFO(typeid(std::tuple_element_t<0, decltype(args)>).name());
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, decltype(alloc) const&>);
                REQUIRE(std::tuple_size_v<decltype(args)> == 1);
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
        using Type = pw::test::OpTrackerAllocatorLast<TestAllocator>;
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
        using Type = pw::test::OpTrackerAllocatorOnly;
        TestAllocator alloc;

        WHEN("called with no additional arguments")
        {
            auto counter_before = Type::getCounter();
            auto args           = pw::uses_allocator_construction_args<Type>(alloc);

            THEN("returns (alloc)")
            {
                REQUIRE(std::tuple_size_v<decltype(args)> == 1);
                INFO("tuple0 = " << typeid(std::tuple_element_t<0, decltype(args)>).name()
                                 << " TestAllocator = " << typeid(TestAllocator).name());
                REQUIRE(std::is_same_v<std::tuple_element_t<0, decltype(args)>, TestAllocator const&>);
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

        WHEN("constructing OpTrackerAllocatorNone with arguments")
        {
            auto counter_before = pw::test::OpTrackerAllocatorNone::getCounter();
            auto args = pw::uses_allocator_construction_args<pw::test::OpTrackerAllocatorNone>(alloc, 42, 10);
            auto counter_after = pw::test::OpTrackerAllocatorNone::getCounter();
            auto diff          = counter_after - counter_before;

            THEN("no allocator style operations are tracked yet")
            {
                REQUIRE(diff.getNoAllocator() == 0);
                REQUIRE(diff.getAllocatorFirst() == 0);
                REQUIRE(diff.getAllocatorLast() == 0);
                REQUIRE(diff.getAllocatorOnly() == 0);
            }
        }

        WHEN("constructing OpTrackerAllocatorFirst with arguments")
        {
            using Type          = pw::test::OpTrackerAllocatorFirst<TestAllocator>;
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