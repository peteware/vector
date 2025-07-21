#include <pw/vector>
#include <catch2/catch.hpp>
#include <stdexcept>
#include <new>

namespace {
    // Helper class that throws on construction after N instances
    class ThrowingType {
    public:
        static int construction_count;
        static int throw_after_n;
        static bool should_throw_on_copy;
        static bool should_throw_on_move;
        
        int value;
        
        explicit ThrowingType(int v = 0) : value(v) {
            if (++construction_count > throw_after_n && throw_after_n >= 0) {
                --construction_count;
                throw std::runtime_error("Construction limit reached");
            }
        }
        
        ThrowingType(const ThrowingType& other) : value(other.value) {
            if (should_throw_on_copy || (++construction_count > throw_after_n && throw_after_n >= 0)) {
                --construction_count;
                throw std::runtime_error("Copy construction failed");
            }
        }
        
        ThrowingType(ThrowingType&& other) noexcept(false) : value(other.value) {
            if (should_throw_on_move || (++construction_count > throw_after_n && throw_after_n >= 0)) {
                --construction_count;
                throw std::runtime_error("Move construction failed");
            }
        }
        
        ~ThrowingType() { --construction_count; }
        
        static void reset() {
            construction_count = 0;
            throw_after_n = -1;
            should_throw_on_copy = false;
            should_throw_on_move = false;
        }
    };
    
    int ThrowingType::construction_count = 0;
    int ThrowingType::throw_after_n = -1;
    bool ThrowingType::should_throw_on_copy = false;
    bool ThrowingType::should_throw_on_move = false;
    
    // Allocator that can throw on allocation
    template<typename T>
    class ThrowingAllocator {
    public:
        using value_type = T;
        static bool should_throw_on_allocate;
        static int allocate_count;
        static int throw_after_n_allocations;
        
        ThrowingAllocator() = default;
        template<typename U> ThrowingAllocator(const ThrowingAllocator<U>&) {}
        
        T* allocate(std::size_t n) {
            if (should_throw_on_allocate || (++allocate_count > throw_after_n_allocations && throw_after_n_allocations >= 0)) {
                throw std::bad_alloc();
            }
            return static_cast<T*>(::operator new(n * sizeof(T)));
        }
        
        void deallocate(T* p, std::size_t) noexcept {
            ::operator delete(p);
        }
        
        bool operator==(const ThrowingAllocator&) const { return true; }
        
        static void reset() {
            should_throw_on_allocate = false;
            allocate_count = 0;
            throw_after_n_allocations = -1;
        }
    };
    
    template<typename T>
    bool ThrowingAllocator<T>::should_throw_on_allocate = false;
    template<typename T>
    int ThrowingAllocator<T>::allocate_count = 0;
    template<typename T>
    int ThrowingAllocator<T>::throw_after_n_allocations = -1;

class MultiExceptionType {
public:
    static int construction_count;
    static bool throw_bad_alloc;
    static bool throw_runtime_error;

    MultiExceptionType() {
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

    static void reset() {
        construction_count = 0;
        throw_bad_alloc = false;
        throw_runtime_error = false;
    }
};
int MultiExceptionType::construction_count = 0;
bool MultiExceptionType::throw_bad_alloc = false;
bool MultiExceptionType::throw_runtime_error = false;
}

TEST_CASE("Constructor Exception Safety", "[vector][constructor][exception_safety]")
{
    using Vector = pw::vector<ThrowingType, ThrowingAllocator<ThrowingType>>;
    ThrowingAllocator<ThrowingType> alloc;
    
    SECTION("Default constructor - allocator throws") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        ThrowingAllocator<ThrowingType>::should_throw_on_allocate = true;
        
        // Default constructor shouldn't allocate, so this should succeed
        REQUIRE_NOTHROW(Vector(alloc));
    }
    
    SECTION("Count constructor - allocation failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        ThrowingAllocator<ThrowingType>::should_throw_on_allocate = true;
        
        REQUIRE_THROWS_AS(Vector(5, alloc), std::bad_alloc);
        
        // Verify no objects were constructed when allocation fails
        REQUIRE(ThrowingType::construction_count == 0);
    }
    
    SECTION("Count constructor - element construction failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        ThrowingType::throw_after_n = 3;  // Allow 3 constructions, fail on 4th
        
        REQUIRE_THROWS_AS(Vector(5, alloc), std::runtime_error);
        
        // Verify cleanup: all successfully constructed objects should be destroyed
        REQUIRE(ThrowingType::construction_count == 0);
    }
    
    SECTION("Count + value constructor - element copy failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        ThrowingType value(42);
        ThrowingType::throw_after_n = 3;  // Fail after 3 more constructions (value already constructed)
        
        REQUIRE_THROWS_AS(Vector(5, value, alloc), std::runtime_error);
        
        // Only the original 'value' object should remain
        REQUIRE(ThrowingType::construction_count == 1);
    }
    
    SECTION("Copy constructor - allocation failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        Vector original({ThrowingType(1), ThrowingType(2), ThrowingType(3)}, alloc);
        
        ThrowingAllocator<ThrowingType>::should_throw_on_allocate = true;
        
        REQUIRE_THROWS_AS(Vector(original), std::bad_alloc);
        
        // Original should be unchanged, no new objects created
        REQUIRE(original.size() == 3);
        REQUIRE(ThrowingType::construction_count == 3);  // Only original's elements
    }
    
    SECTION("Copy constructor - element copy failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        Vector original({ThrowingType(1), ThrowingType(2), ThrowingType(3)}, alloc);
        int original_count = ThrowingType::construction_count;
        
        ThrowingType::throw_after_n = original_count + 1;  // Fail on 2nd copy
        
        REQUIRE_THROWS_AS(Vector(original), std::runtime_error);
        
        // Original unchanged, no leaked objects from partial copy
        REQUIRE(original.size() == 3);
        REQUIRE(ThrowingType::construction_count == original_count);
    }
    
    SECTION("Move constructor - should not throw with noexcept(false) elements") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        Vector original({ThrowingType(1), ThrowingType(2), ThrowingType(3)}, alloc);
        
        // Move constructor should succeed even with throwing move constructor
        // because it should use allocation + copy instead of direct move
        REQUIRE_NOTHROW(Vector(std::move(original)));
    }
    
    SECTION("Iterator constructor - allocation failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        ThrowingType data[] = {ThrowingType(1), ThrowingType(2), ThrowingType(3)};
        
        ThrowingAllocator<ThrowingType>::should_throw_on_allocate = true;
        
        REQUIRE_THROWS_AS(Vector(&data[0], &data[3], alloc), std::bad_alloc);
        
        // Original data unchanged, no new objects created
        REQUIRE(ThrowingType::construction_count == 3);
    }
    
    SECTION("Iterator constructor - element construction failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        ThrowingType data[] = {ThrowingType(1), ThrowingType(2), ThrowingType(3)};
        int original_count = ThrowingType::construction_count;
        
        ThrowingType::throw_after_n = original_count + 1;  // Fail on 2nd copy
        
        REQUIRE_THROWS_AS(Vector(&data[0], &data[3], alloc), std::runtime_error);
        
        // Original data unchanged, no leaked objects
        REQUIRE(ThrowingType::construction_count == original_count);
    }
    
    SECTION("Initializer list constructor - allocation failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        ThrowingAllocator<ThrowingType>::should_throw_on_allocate = true;
        
        REQUIRE_THROWS_AS(Vector({ThrowingType(1), ThrowingType(2)}, alloc), std::bad_alloc);
        
        // All temporary objects from initializer list should be cleaned up
        REQUIRE(ThrowingType::construction_count == 0);
    }
    
    SECTION("Strong exception guarantee - vector state unchanged on failure") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        
        // This tests that if a constructor throws, it doesn't leave
        // the vector in a partially constructed state
        try {
            ThrowingType::throw_after_n = 2;
            Vector v(5, alloc);  // Should throw after constructing 2 elements
            FAIL("Expected exception was not thrown");
        } catch (const std::runtime_error&) {
            // Exception expected - verify cleanup
            REQUIRE(ThrowingType::construction_count == 0);
        }
    }
}

TEST_CASE("Constructor Exception Safety - Resource Cleanup", "[vector][constructor][cleanup]")
{
    SECTION("Verify no memory leaks on constructor exception") {
        // This would typically be verified with tools like valgrind or AddressSanitizer
        // but we can at least check object counts
        
        auto initial_count = ThrowingType::construction_count;
        
        try {
            ThrowingType::reset();
            ThrowingType::throw_after_n = 3;
            
            pw::vector<ThrowingType> v(10);  // Should throw after 3 constructions
            FAIL("Expected exception");
        } catch (const std::runtime_error&) {
            // All objects should be destroyed
            REQUIRE(ThrowingType::construction_count == 0);
        }
    }
    
    SECTION("Exception safety with different allocation patterns") {
        ThrowingAllocator<int>::reset();
        
        // Test allocation failure on different constructor paths
        SECTION("Large allocation request") {
            using Vector = pw::vector<int, ThrowingAllocator<int>>;
            ThrowingAllocator<int> alloc;
            ThrowingAllocator<int>::throw_after_n_allocations = 0;
            
            REQUIRE_THROWS_AS((Vector{static_cast<Vector::size_type>(100000), alloc}), std::bad_alloc);
        }
        
        SECTION("Multiple small allocations") {
            ThrowingAllocator<int> alloc;
            ThrowingAllocator<int>::throw_after_n_allocations = 2;
            
            // Create a vector that might need reallocation
            try {
                pw::vector<int, ThrowingAllocator<int>> v(alloc);
                v.reserve(10);   // First allocation
                v.reserve(100);  // Second allocation  
                v.reserve(1000); // Third allocation should fail
                FAIL("Expected exception");
            } catch (const std::bad_alloc&) {
                // Expected
            }
        }
    }
}

TEST_CASE("Constructor Exception Safety - Edge Cases", "[vector][constructor][edge_cases]")
{
    SECTION("Zero size with throwing allocator") {
        ThrowingAllocator<ThrowingType>::reset();
        ThrowingType::reset();
        ThrowingAllocator<ThrowingType>::should_throw_on_allocate = true;
        
        // Zero-size constructor should not allocate
        REQUIRE_NOTHROW(pw::vector<ThrowingType, ThrowingAllocator<ThrowingType>>(0, ThrowingAllocator<ThrowingType>{}));
    }
    
    SECTION("Exception in destructor during cleanup") {
        // Test that if destructor throws during exception cleanup,
        // the program still handles it gracefully (though this may terminate)
        ThrowingType::reset();
        
        // This is mainly a documentation of behavior rather than a strict test
        // since destructors throwing during stack unwinding is undefined behavior
        INFO("Destructor exceptions during cleanup are undefined behavior");
    }
    
    SECTION("Multiple exception types") {

        MultiExceptionType::reset();
        MultiExceptionType::throw_runtime_error = true;
        
        REQUIRE_THROWS_AS((pw::vector<MultiExceptionType>(5)), std::runtime_error);
        REQUIRE(MultiExceptionType::construction_count == 0);
    }
}