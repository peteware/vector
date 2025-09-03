#ifndef INCLUDED_PW_TEST_ALLOCATOR_FIRST_TYPE_H
#define INCLUDED_PW_TEST_ALLOCATOR_FIRST_TYPE_H

#include <pw/impl/allocator.h>

#include <memory>
#include <test_optracker.h>

namespace pw::test {

template<typename Alloc = pw::allocator<int>>
struct AllocatorFirstType : public OpTracker
{
    using allocator_type = Alloc;

    static OpCounter getCounter();

    //AllocatorFirstType() = default;
    AllocatorFirstType(std::allocator_arg_t, Alloc const& alloc)
        : OpTracker(s_opCounter)
        , m_allocator(alloc)
    {
        s_opCounter.addDefaultConstructorAlloc().addAllocatorFirst();
    }

    AllocatorFirstType(AllocatorFirstType const& copy, Alloc const& alloc)
        : OpTracker(s_opCounter)
        , m_allocator(alloc)
    {
        s_opCounter.addCopyConstructorAlloc();
    }
    AllocatorFirstType(std::allocator_arg_t, Alloc const& alloc, value_type const& value)
        : OpTracker(s_opCounter, value)
        , m_allocator(alloc)
    {
        s_opCounter.addOtherConstructor().addAllocatorFirst();
    }

    AllocatorFirstType(std::allocator_arg_t, Alloc const& alloc, value_type const& value, int extra)
        : OpTracker(s_opCounter, value + extra)
        , m_allocator(alloc)
    {
        s_opCounter.addOtherConstructor().addAllocatorFirst();
    }

private:
    static OpCounter s_opCounter;
    allocator_type   m_allocator;
};

template<typename Alloc>
OpCounter AllocatorFirstType<Alloc>::s_opCounter;

template<typename Alloc>
OpCounter
AllocatorFirstType<Alloc>::getCounter()
{
    return s_opCounter;
}

} // namespace pw::test

namespace std {
template<typename Alloc>
struct uses_allocator<pw::test::AllocatorFirstType<Alloc>, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_ALLOCATOR_FIRST_TYPE_H */