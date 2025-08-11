#include <catch2/catch.hpp>
#include <new>
#include <pw/vector>
#include <stdexcept>
#include <test_throwing_allocator.h>
#include <test_throwingtype.h>

namespace {
class MultiExceptionType
{
public:
    static int  construction_count;
    static bool throw_bad_alloc;
    static bool throw_runtime_error;

    MultiExceptionType()
    {
        ++construction_count;
        if (throw_bad_alloc)
        {
            --construction_count;
            throw std::bad_alloc();
        }
        if (throw_runtime_error)
        {
            --construction_count;
            throw std::runtime_error("test");
        }
    }

    ~MultiExceptionType() { --construction_count; }

    static void reset()
    {
        construction_count  = 0;
        throw_bad_alloc     = false;
        throw_runtime_error = false;
    }
};
int  MultiExceptionType::construction_count  = 0;
bool MultiExceptionType::throw_bad_alloc     = false;
bool MultiExceptionType::throw_runtime_error = false;
} // namespace

TEST_CASE("Constructor Exception Safety", "[vector][constructor][exception_safety]")
{
    using ThrowingType      = pw::test::ThrowingType;
    using ThrowingAllocator = pw::test::ThrowingAllocator<ThrowingType>;
    using Vector            = pw::vector<ThrowingType, ThrowingAllocator>;
    ThrowingAllocator alloc;

    SECTION("Default constructor - allocator throws")
    {
        alloc.reset();
        ThrowingType::reset();
        alloc.should_throw_on_allocate = true;

        // Default constructor shouldn't allocate, so this should succeed
        REQUIRE_NOTHROW(Vector(alloc));
    }

    SECTION("Count constructor - allocation failure")
    {
        alloc.reset();
        ThrowingType::reset();
        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(5, alloc), std::bad_alloc);

        // Verify no objects were constructed when allocation fails
        REQUIRE(ThrowingType::construction_count == 0);
    }

    SECTION("Count constructor - element construction failure")
    {
        alloc.reset();
        ThrowingType::reset();
        ThrowingType::throw_after_n = 3; // Allow 3 constructions, fail on 4th

        REQUIRE_THROWS_AS(Vector(5, alloc), std::runtime_error);

        // Verify cleanup: all successfully constructed objects should be destroyed
        REQUIRE(ThrowingType::construction_count == 0);
    }

    SECTION("Count + value constructor - element copy failure")
    {
        alloc.reset();
        ThrowingType::reset();
        ThrowingType value(42);
        ThrowingType::throw_after_n = 3; // Fail after 3 more constructions (value already constructed)

        REQUIRE_THROWS_AS(Vector(5, value, alloc), std::runtime_error);

        // Only the original 'value' object should remain
        REQUIRE(ThrowingType::construction_count == 1);
    }

    SECTION("Copy constructor - allocation failure")
    {
        alloc.reset();
        ThrowingType::reset();

        Vector original({ ThrowingType(1), ThrowingType(2), ThrowingType(3) }, alloc);

        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(original), std::bad_alloc);

        // Original should be unchanged, no new objects created
        REQUIRE(original.size() == 3);
        REQUIRE(ThrowingType::construction_count == 3); // Only original's elements
    }

    SECTION("Copy constructor - element copy failure")
    {
        alloc.reset();
        ThrowingType::reset();

        Vector original({ ThrowingType(1), ThrowingType(2), ThrowingType(3) }, alloc);
        int    original_count       = ThrowingType::construction_count;

        ThrowingType::throw_after_n = original_count + 1; // Fail on 2nd copy

        REQUIRE_THROWS_AS(Vector(original), std::runtime_error);

        // Original unchanged, no leaked objects from partial copy
        REQUIRE(original.size() == 3);
        REQUIRE(ThrowingType::construction_count == original_count);
    }

    SECTION("Move constructor - should not throw with noexcept(false) elements")
    {
        alloc.reset();
        ThrowingType::reset();

        Vector original({ ThrowingType(1), ThrowingType(2), ThrowingType(3) }, alloc);

        // Move constructor should succeed even with throwing move constructor
        // because it should use allocation + copy instead of direct move
        REQUIRE_NOTHROW(Vector(std::move(original)));
    }

    SECTION("Iterator constructor - allocation failure")
    {
        alloc.reset();
        ThrowingType::reset();

        ThrowingType data[]            = { ThrowingType(1), ThrowingType(2), ThrowingType(3) };

        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector(&data[0], &data[3], alloc), std::bad_alloc);

        // Original data unchanged, no new objects created
        REQUIRE(ThrowingType::construction_count == 3);
    }

    SECTION("Iterator constructor - element construction failure")
    {
        alloc.reset();
        ThrowingType::reset();

        ThrowingType data[]         = { ThrowingType(1), ThrowingType(2), ThrowingType(3) };
        int          original_count = ThrowingType::construction_count;

        ThrowingType::throw_after_n = original_count + 1; // Fail on 2nd copy

        REQUIRE_THROWS_AS(Vector(&data[0], &data[3], alloc), std::runtime_error);

        // Original data unchanged, no leaked objects
        REQUIRE(ThrowingType::construction_count == original_count);
    }

    SECTION("Initializer list constructor - allocation failure")
    {
        alloc.reset();
        ThrowingType::reset();

        alloc.should_throw_on_allocate = true;

        REQUIRE_THROWS_AS(Vector({ ThrowingType(1), ThrowingType(2) }, alloc), std::bad_alloc);

        // All temporary objects from initializer list should be cleaned up
        REQUIRE(ThrowingType::construction_count == 0);
    }

    SECTION("Strong exception guarantee - vector state unchanged on failure")
    {
        alloc.reset();
        ThrowingType::reset();

        // This tests that if a constructor throws, it doesn't leave
        // the vector in a partially constructed state
        try
        {
            ThrowingType::throw_after_n = 2;
            Vector v(5, alloc); // Should throw after constructing 2 elements
            FAIL("Expected exception was not thrown");
        }
        catch (const std::runtime_error&)
        {
            // Exception expected - verify cleanup
            REQUIRE(ThrowingType::construction_count == 0);
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
    SECTION("Zero size with throwing allocator")
    {
        using ThrowingType      = pw::test::ThrowingType;
        using ThrowingAllocator = pw::test::ThrowingAllocator<ThrowingType>;
        ThrowingAllocator alloc;
        alloc.reset();
        ThrowingType::reset();
        alloc.should_throw_on_allocate = true;

        // Zero-size constructor should not allocate
        REQUIRE_NOTHROW(pw::vector<ThrowingType, ThrowingAllocator>(0, alloc));
    }

    SECTION("Exception in destructor during cleanup")
    {
        // Test that if destructor throws during exception cleanup,
        // the program still handles it gracefully (though this may terminate)
        pw::test::ThrowingType::reset();

        // This is mainly a documentation of behavior rather than a strict test
        // since destructors throwing during stack unwinding is undefined behavior
        INFO("Destructor exceptions during cleanup are undefined behavior");
    }

    SECTION("Multiple exception types")
    {
        MultiExceptionType::reset();
        MultiExceptionType::throw_runtime_error = true;

        REQUIRE_THROWS_AS(pw::vector<MultiExceptionType>(5), std::runtime_error);
        REQUIRE(MultiExceptionType::construction_count == 0);
    }
}