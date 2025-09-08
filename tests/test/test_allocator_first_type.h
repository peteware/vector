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
    explicit AllocatorFirstType(allocator_type const& alloc = allocator_type())
        : OpTracker(s_opCounter)
        , m_allocator(alloc)
    {
        s_opCounter.addCopyConstructorAlloc();
    }

    AllocatorFirstType(AllocatorFirstType const& other)
        : OpTracker(s_opCounter)
        , m_allocator()
    {
    }

    AllocatorFirstType(value_type const& value, Alloc const& alloc)
        : OpTracker(s_opCounter, value)
        , m_allocator(alloc)
    {
        s_opCounter.addOtherConstructor().addAllocatorFirst();
    }

    AllocatorFirstType(value_type const& value, int extra, Alloc const& alloc)
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
template<typename Alloc1, typename Alloc2>
struct uses_allocator<pw::test::AllocatorFirstType<Alloc1>, Alloc2> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_ALLOCATOR_FIRST_TYPE_H */