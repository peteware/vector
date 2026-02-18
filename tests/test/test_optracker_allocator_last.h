#ifndef INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_LAST_H
#define INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_LAST_H

#include <memory>
#include <test_optracker.h>

namespace pw::test {

template<typename Alloc>
struct OpTrackerAllocatorLast : OpTracker
{
    using allocator_type = Alloc;

    static OpCounter& getCounter();

    OpTrackerAllocatorLast();
    explicit OpTrackerAllocatorLast(allocator_type const& alloc);
    OpTrackerAllocatorLast(OpTrackerAllocatorLast const& other, allocator_type const& alloc);
    OpTrackerAllocatorLast(value_type const& value, allocator_type const& alloc);
    OpTrackerAllocatorLast(value_type const& value, int extra, allocator_type const& alloc);

private:
    static OpCounter s_opCounter;
    allocator_type   m_allocator;
};

template<typename Alloc>
OpCounter OpTrackerAllocatorLast<Alloc>::s_opCounter;

template<typename Alloc>
OpCounter&
OpTrackerAllocatorLast<Alloc>::getCounter()
{
    return s_opCounter;
}

template<typename Alloc>
OpTrackerAllocatorLast<Alloc>::OpTrackerAllocatorLast()
    : OpTracker(s_opCounter)
    , m_allocator()
{
    s_opCounter.addDefaultConstructor().addNoAllocator();
}

template<typename Alloc>
OpTrackerAllocatorLast<Alloc>::OpTrackerAllocatorLast(allocator_type const& alloc)
    : OpTracker(s_opCounter)
    , m_allocator(alloc)
{
    s_opCounter.addDefaultConstructor().addAllocatorLast();
}

template<typename Alloc>
OpTrackerAllocatorLast<Alloc>::OpTrackerAllocatorLast(OpTrackerAllocatorLast const& other,
                                                      allocator_type const&         alloc)
    : OpTracker(other)
    , m_allocator(alloc)
{
    s_opCounter.addCopyConstructorAlloc().addAllocatorLast();
}

template<typename Alloc>
OpTrackerAllocatorLast<Alloc>::OpTrackerAllocatorLast(value_type const& value, allocator_type const& alloc)
    : OpTracker(s_opCounter, value)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorLast();
}

template<typename Alloc>
OpTrackerAllocatorLast<Alloc>::OpTrackerAllocatorLast(value_type const&     value,
                                                      int                   extra,
                                                      allocator_type const& alloc)
    : OpTracker(s_opCounter, value + extra)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorLast();
}

} // namespace pw::test

namespace std {
template<typename Alloc>
struct uses_allocator<pw::test::OpTrackerAllocatorLast<Alloc>, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_LAST_H */