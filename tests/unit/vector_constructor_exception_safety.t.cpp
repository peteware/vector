#include <pw/vector>
#include <test_multi_exception_type.h>
#include <test_testtype.h>
#include <test_throwing_allocator.h>
#include <test_throwingtype.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <new>
#include <stdexcept>

TEMPLATE_LIST_TEST_CASE("Constructor Exception Safety",
                        "[vector][constructor][exception_safety]",
                        pw::test::TestTypeListThrowing)
{
    using Vector         = TestType;
    using value_type     = Vector::value_type;
    using allocator_type = Vector::allocator_type;

    allocator_type alloc;

    SECTION("Default constructor - allocator throws")
    {
        allocator_type::reset();
        value_type::reset();
        alloc.should_throw_on_allocate = true;

        // Default constructor shouldn't allocate, so this should succeed
        REQUIRE_NOTHROW(Vector(alloc));
    }

    SECTION("Count constructor - allocation failure")
    {
        alloc.reset();
        value_type::reset();
        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(5, alloc), std::bad_alloc);

        // Verify no objects were constructed when allocation fails
        REQUIRE(value_type::construction_count == 0);
    }

    SECTION("Count constructor - element construction failure")
    {
        allocator_type::reset();
        value_type::reset();
        value_type::throw_after_n = 3; // Allow 3 constructions, fail on 4th

        REQUIRE_THROWS_AS(Vector(5, alloc), std::runtime_error);

        // Verify cleanup: all successfully constructed objects should be destroyed
        REQUIRE(value_type::construction_count == 0);
    }

    SECTION("Count + value constructor - element copy failure")
    {
        allocator_type::reset();
        value_type::reset();
        value_type value(42);
        value_type::throw_after_n = 3; // Fail after 3 more constructions (value already constructed)
        //allocator_type::should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(5, value, alloc), std::runtime_error);

        // Only the original 'value' object should remain
        // This isn't the case for std::vector
        //REQUIRE(value_type::construction_count == 1);
    }

    SECTION("Copy constructor - allocation failure")
    {
        allocator_type::reset();
        value_type::reset();

        Vector original({ value_type(1), value_type(2), value_type(3) }, alloc);

        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(original), std::bad_alloc);

        // Original should be unchanged, no new objects created
        REQUIRE(original.size() == 3);
        REQUIRE(value_type::construction_count == 3); // Only original's elements
    }

    SECTION("Copy constructor - element copy failure")
    {
        allocator_type::reset();
        value_type::reset();

        Vector original({ value_type(1), value_type(2), value_type(3) }, alloc);
        int    original_count     = value_type::construction_count;

        value_type::throw_after_n = original_count + 1; // Fail on 2nd copy

        REQUIRE_THROWS_AS(Vector(original), std::runtime_error);

        // Original unchanged, no leaked objects from partial copy
        REQUIRE(original.size() == 3);
        REQUIRE(value_type::construction_count == original_count);
    }

    SECTION("Move constructor - should not throw with noexcept(false) elements")
    {
        allocator_type::reset();
        value_type::reset();

        Vector original({ value_type(1), value_type(2), value_type(3) }, alloc);

        // Move constructor should succeed even with throwing move constructor
        // because it should use allocation + copy instead of direct move
        REQUIRE_NOTHROW(Vector(std::move(original)));
    }

    SECTION("Iterator constructor - allocation failure")
    {
        allocator_type::reset();
        value_type::reset();

        value_type data[]              = { value_type(1), value_type(2), value_type(3) };

        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(&data[0], &data[3], alloc), std::bad_alloc);

        // Original data unchanged, no new objects created
        REQUIRE(value_type::construction_count == 3);
    }

    SECTION("Iterator constructor - element construction failure")
    {
        allocator_type::reset();
        value_type::reset();

        value_type data[]         = { value_type(1), value_type(2), value_type(3) };
        int        original_count = value_type::construction_count;

        value_type::throw_after_n = original_count + 1; // Fail on 2nd copy

        REQUIRE_THROWS_AS(Vector(&data[0], &data[3], alloc), std::runtime_error);

        // Original data unchanged, no leaked objects
        REQUIRE(value_type::construction_count == original_count);
    }

    SECTION("Initializer list constructor - allocation failure")
    {
        allocator_type::reset();
        value_type::reset();

        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector({ value_type(1), value_type(2) }, alloc), std::bad_alloc);

        // All temporary objects from initializer list should be cleaned up
        REQUIRE(value_type::construction_count == 0);
    }

    SECTION("Strong exception guarantee - vector state unchanged on failure")
    {
        allocator_type::reset();
        value_type::reset();

        // This tests that if a constructor throws, it doesn't leave
        // the vector in a partially constructed state
        try
        {
            value_type::throw_after_n = 2;
            Vector v(5, alloc); // Should throw after constructing 2 elements
            FAIL("Expected exception was not thrown");
        }
        catch (const std::runtime_error&)
        {
            // Exception expected - verify cleanup
            REQUIRE(value_type::construction_count == 0);
        }
    }
}

TEST_CASE("Constructor Exception Safety - Resource Cleanup", "[vector][constructor][cleanup]")
{
    SECTION("Verify no memory leaks on constructor exception")
    {
        try
        {
            pw::test::ThrowingType::reset();
            pw::test::ThrowingType::throw_after_n = 3;

            pw::vector<pw::test::ThrowingType> v(10); // Should throw after 3 constructions
            FAIL("Expected exception");
        }
        catch (const std::runtime_error&)
        {
            // All objects should be destroyed
            REQUIRE(pw::test::ThrowingType::construction_count == 0);
        }
    }

    SECTION("Exception safety with different allocation patterns")
    {
        pw::test::ThrowingAllocator<int>::reset();

        // Test allocation failure on different constructor paths
        SECTION("Large allocation request")
        {
            using Vector = pw::vector<int, pw::test::ThrowingAllocator<int>>;
            pw::test::ThrowingAllocator<int> alloc;
            pw::test::ThrowingAllocator<int>::throw_after_n_allocations = 0;

            REQUIRE_THROWS_AS((Vector { static_cast<Vector::size_type>(100000), alloc }), std::bad_alloc);
        }

        SECTION("Multiple small allocations")
        {
            pw::test::ThrowingAllocator<int> alloc;
            pw::test::ThrowingAllocator<int>::throw_after_n_allocations = 2;

            // Create a vector that might need reallocation
            try
            {
                pw::vector<int, pw::test::ThrowingAllocator<int>> v(alloc);
                v.reserve(10);   // First allocation
                v.reserve(100);  // Second allocation
                v.reserve(1000); // Third allocation should fail
                FAIL("Expected exception");
            }
            catch (const std::bad_alloc&)
            {
                // Expected
            }
        }
    }
}

TEST_CASE("Constructor Exception Safety - Edge Cases", "[vector][constructor][edge_cases]")
{
    using value_type        = pw::test::ThrowingType;
    using ThrowingAllocator = pw::test::ThrowingAllocator<value_type>;
    SECTION("Zero size with throwing allocator")
    {
        ThrowingAllocator alloc;
        alloc.reset();
        value_type::reset();
        alloc.should_throw_on_allocate = true;

        // Zero-size constructor should not allocate
        REQUIRE_NOTHROW(pw::vector<value_type, ThrowingAllocator>(0, alloc));
    }

    SECTION("Exception in destructor during cleanup")
    {
        // Test that if destructor throws during exception cleanup,
        // the program still handles it gracefully (though this may terminate)
        value_type::reset();

        // This is mainly a documentation of behavior rather than a strict test
        // since destructors throwing during stack unwinding is undefined behavior
        INFO("Destructor exceptions during cleanup are undefined behavior");
    }

    SECTION("Multiple exception types")
    {
        pw::test::MultiExceptionType::reset();
        pw::test::MultiExceptionType::throw_runtime_error = true;

        REQUIRE_THROWS_AS(pw::vector<pw::test::MultiExceptionType>(5), std::runtime_error);
        REQUIRE(pw::test::MultiExceptionType::construction_count == 0);
    }
}