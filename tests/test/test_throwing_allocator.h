#ifndef PW_TEST_THROWING_ALLOCATOR_H
#define PW_TEST_THROWING_ALLOCATOR_H

#include <new>
#include <cstddef>

namespace pw {
namespace test {

// Allocator that can throw on allocation
template<typename T>
class ThrowingAllocator
{
public:
    using value_type = T;
    static bool should_throw_on_allocate;
    static int  allocate_count;
    static int  throw_after_n_allocations;

    ThrowingAllocator() = default;
    template<typename U>
    ThrowingAllocator(const ThrowingAllocator<U>&)
    {
    }

    T* allocate(std::size_t n)
    {
        if (should_throw_on_allocate ||
            (++allocate_count > throw_after_n_allocations && throw_after_n_allocations >= 0))
        {
            throw std::bad_alloc();
        }
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }

    bool operator==(const ThrowingAllocator&) const { return true; }

    static void reset()
    {
        should_throw_on_allocate  = false;
        allocate_count            = 0;
        throw_after_n_allocations = -1;
    }
};

// Static member definitions
template<typename T>
inline bool ThrowingAllocator<T>::should_throw_on_allocate = false;
template<typename T>
inline int ThrowingAllocator<T>::allocate_count = 0;
template<typename T>
inline int ThrowingAllocator<T>::throw_after_n_allocations = -1;

} // namespace test
} // namespace pw

#endif // PW_TEST_THROWING_ALLOCATOR_H