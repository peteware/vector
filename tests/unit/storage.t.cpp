#include <pw/impl/allocator/allocator.h>
#include <pw/internal/storage.h>
#include <tests/test/test_opcounter.h>
#include <tests/test/test_optracker.h>
#include <tests/test/test_throwingtype.h>
// ReSharper disable once CppUnusedIncludeDirective
//#include <pw/internal/rsize_fix.h>

#include <catch2/catch_test_macros.hpp>
using namespace pw::test;

SCENARIO("Storage constructor sets up capacity", "[storage]")
{
    GIVEN("A Storage instance with capacity memory")
    {
        pw::internal::Storage<int> storage(pw::allocator<int> {}, 5);

        WHEN("The size() is 0")
        {
            REQUIRE(storage.empty());
            THEN("the capacity size is correct")
            {
                REQUIRE(storage.capacity() == 5);
                REQUIRE(storage.end() == storage.begin());
                REQUIRE(storage.capacity_begin() == storage.begin());
                REQUIRE(storage.capacity_end() == storage.begin() + 5);
            }
        }
        WHEN("This size is 1")
        {
            storage.uninitialized_default_construct(storage.capacity_begin(), storage.capacity_begin() + 1)
                .set_size(1);
            THEN("the capacity size is correct")
            {
                REQUIRE(storage.capacity_begin() == storage.begin() + 1);
                REQUIRE(storage.capacity_end() == storage.begin() + 5);
            }
        }
    }
}
SCENARIO("Storage::uninitialized_fill() constructs objects with allocator_traits", "[storage]")
{
    GIVEN("A Storage instance with capacity memory")
    {
        pw::internal::Storage<int> storage(pw::allocator<int> {}, 5);

        WHEN("uninitialized_fill is called with a value")
        {
            storage.uninitialized_fill(storage.begin(), storage.begin() + 3, 42);
            storage.set_size(3);

            THEN("the elements are properly constructed")
            {
                REQUIRE(storage.size() == 3);
                REQUIRE(storage.begin()[0] == 42);
                REQUIRE(storage.begin()[1] == 42);
                REQUIRE(storage.begin()[2] == 42);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_fill() provides exception safety", "[storage]")
{
    using pw::test::ThrowingType;

    GIVEN("A Storage instance with ThrowingType elements")
    {
        pw::internal::Storage<ThrowingType> storage(pw::allocator<ThrowingType> {}, 5);
        ThrowingType::reset();

        WHEN("ThrowingType throws after 2 constructions")
        {
            ThrowingType const value(99);
            ThrowingType::reset();           // Reset count after creating value
            ThrowingType::throw_after_n = 2; // Set throw limit after reset

            REQUIRE_THROWS(storage.uninitialized_fill(storage.begin(), storage.begin() + 4, value));

            THEN("all constructed objects are properly destroyed")
            {
                REQUIRE(ThrowingType::construction_count == 0);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_copy() copies objects using allocator_traits", "[storage]")
{
    GIVEN("Source data and target Storage")
    {
        int                        source[] = { 1, 2, 3, 4, 5 };
        pw::internal::Storage<int> storage(pw::allocator<int> {}, 5);

        WHEN("uninitialized_copy is called")
        {
            storage.uninitialized_copy(source, source + 3, storage.begin());
            storage.set_size(3);

            THEN("elements are properly copied")
            {
                REQUIRE(storage.size() == 3);
                REQUIRE(storage.begin()[0] == 1);
                REQUIRE(storage.begin()[1] == 2);
                REQUIRE(storage.begin()[2] == 3);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_copy() provides exception safety", "[storage]")
{
    using pw::test::ThrowingType;

    GIVEN("Source ThrowingType array and target Storage")
    {
        ThrowingType::reset();
        ThrowingType::throw_after_n = -1; // No exceptions for source creation
        ThrowingType source[4]      = { ThrowingType(1), ThrowingType(2), ThrowingType(3), ThrowingType(4) };
        pw::internal::Storage<ThrowingType> storage(pw::allocator<ThrowingType> {}, 5);
        ThrowingType::reset();

        WHEN("ThrowingType throws after 2 constructions during copy")
        {
            ThrowingType::throw_after_n = 2;

            REQUIRE_THROWS(storage.uninitialized_copy(source, source + 4, storage.begin()));

            THEN("all constructed objects are properly destroyed")
            {
                REQUIRE(ThrowingType::construction_count == 0);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_default_construct() constructs objects using allocator_traits", "[storage]")
{
    GIVEN("A Storage instance with capacity memory")
    {
        pw::internal::Storage<int> storage(pw::allocator<int> {}, 5);

        WHEN("uninitialized_default_construct is called")
        {
            storage.uninitialized_default_construct(storage.begin(), storage.begin() + 3);
            storage.set_size(3);

            THEN("elements are default-constructed")
            {
                REQUIRE(storage.size() == 3);
                // For int, default construction gives 0
                REQUIRE(storage.begin()[0] == 0);
                REQUIRE(storage.begin()[1] == 0);
                REQUIRE(storage.begin()[2] == 0);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_default_construct() provides exception safety", "[storage]")
{
    using pw::test::ThrowingType;

    GIVEN("A Storage instance with ThrowingType elements")
    {
        pw::internal::Storage<ThrowingType> storage(pw::allocator<ThrowingType> {}, 5);
        ThrowingType::reset();

        WHEN("ThrowingType throws after 2 constructions")
        {
            ThrowingType::throw_after_n = 2;

            REQUIRE_THROWS(storage.uninitialized_default_construct(storage.begin(), storage.begin() + 4));

            THEN("all constructed objects are properly destroyed")
            {
                REQUIRE(ThrowingType::construction_count == 0);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_move() moves objects using allocator_traits", "[storage]")
{
    GIVEN("Source Storage and target Storage")
    {
        pw::internal::Storage<int> source_storage(pw::allocator<int> {}, 3);
        source_storage.uninitialized_fill(source_storage.begin(), source_storage.begin() + 3, 42).set_size(3);

        pw::internal::Storage<int> target_storage(pw::allocator<int> {}, 5);

        WHEN("uninitialized_move is called")
        {
            target_storage
                .uninitialized_move(source_storage.begin(), source_storage.end(), target_storage.begin())
                .set_size(3);

            THEN("elements are properly moved")
            {
                REQUIRE(target_storage.size() == 3);
                REQUIRE(target_storage.begin()[0] == 42);
                REQUIRE(target_storage.begin()[1] == 42);
                REQUIRE(target_storage.begin()[2] == 42);
            }
        }
    }
}

SCENARIO("Storage::uninitialized_move() provides exception safety", "[storage]")
{
    using pw::test::ThrowingType;

    GIVEN("Source ThrowingType Storage and target Storage")
    {
        ThrowingType::reset();
        ThrowingType::throw_after_n = -1; // No exceptions for source creation
        pw::internal::Storage<ThrowingType> source_storage(pw::allocator<ThrowingType> {}, 4);
        source_storage.uninitialized_default_construct(source_storage.begin(), source_storage.begin() + 4);
        source_storage.set_size(4);

        pw::internal::Storage<ThrowingType> target_storage(pw::allocator<ThrowingType> {}, 5);
        ThrowingType::reset();

        WHEN("ThrowingType throws after 2 constructions during move")
        {
            ThrowingType::throw_after_n = 2;

            REQUIRE_THROWS(target_storage.uninitialized_move(
                source_storage.begin(), source_storage.end(), target_storage.begin()));

            THEN("all constructed objects are properly destroyed")
            {
                REQUIRE(ThrowingType::construction_count == 0);
            }
        }
    }
}

SCENARIO("Storage::copy() copies objects to initialized memory", "[storage]")
{
    GIVEN("Source data and initialized target Storage")
    {
        int                        source[] = { 10, 20, 30 };
        pw::internal::Storage<int> storage(pw::allocator<int> {}, 5);
        // Initialize with different values first
        storage.uninitialized_fill(storage.begin(), storage.begin() + 5, 0);
        storage.set_size(5);

        WHEN("copy is called")
        {
            auto result = storage.copy(source, source + 3, storage.begin());

            THEN("elements are properly copied and iterator is returned")
            {
                REQUIRE(result == storage.begin() + 3);
                REQUIRE(storage.begin()[0] == 10);
                REQUIRE(storage.begin()[1] == 20);
                REQUIRE(storage.begin()[2] == 30);
                REQUIRE(storage.begin()[3] == 0); // unchanged
                REQUIRE(storage.begin()[4] == 0); // unchanged
            }
        }
    }
}

SCENARIO("Storage member functions work with OpTracker for operation counting", "[storage]")
{
    GIVEN("A Storage instance with OpTracker elements")
    {
        struct TestOpTracker : public OpTracker
        {
            explicit TestOpTracker(OpCounter& opCounter)
                : OpTracker(opCounter)
            {
            }
            TestOpTracker(OpCounter& opCounter, int value)
                : OpTracker(opCounter, value)
            {
            }
            TestOpTracker(TestOpTracker const& other)
                : OpTracker(other)
            {
            }
            TestOpTracker(TestOpTracker&& other) noexcept
                : OpTracker(std::move(other))
            {
            }
            TestOpTracker& operator=(TestOpTracker const& other)
            {
                OpTracker::operator=(other);
                return *this;
            }
            TestOpTracker& operator=(TestOpTracker&& other) noexcept
            {
                OpTracker::operator=(std::move(other));
                return *this;
            }
        };

        OpCounter                            counter;
        pw::internal::Storage<TestOpTracker> storage(pw::allocator<TestOpTracker> {}, 5);

        WHEN("uninitialized_fill is called")
        {
            TestOpTracker value(counter, 42);
            counter = OpCounter {}; // Reset counter after creating value

            storage.uninitialized_fill(storage.begin(), storage.begin() + 3, value);
            storage.set_size(3);

            THEN("copy constructors are called via allocator_traits::construct")
            {
                REQUIRE(counter.getCopyConstructor() == 3);
                REQUIRE(storage.begin()[0].value() == 42);
                REQUIRE(storage.begin()[1].value() == 42);
                REQUIRE(storage.begin()[2].value() == 42);
            }
        }
    }
}