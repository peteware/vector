#ifndef INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_FIRST_H
#define INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_FIRST_H

//#include <pw/impl/allocator.h>

#include <memory>
#include <test_optracker.h>

namespace pw::test {

template<typename Alloc>
struct OpTrackerAllocatorFirst : public OpTracker
{
    using allocator_type = Alloc;

    static OpCounter getCounter();

    OpTrackerAllocatorFirst();
    explicit OpTrackerAllocatorFirst(allocator_type const& alloc);
    OpTrackerAllocatorFirst(std::allocator_arg_t, allocator_type const& alloc, value_type const& value);
    OpTrackerAllocatorFirst(std::allocator_arg_t,
                            allocator_type const& alloc,
                            value_type const&     value,
                            int                   extra);

private:
    static OpCounter s_opCounter;
    allocator_type   m_allocator;
};

template<typename Alloc>
OpCounter OpTrackerAllocatorFirst<Alloc>::s_opCounter;

template<typename Alloc>
OpCounter
OpTrackerAllocatorFirst<Alloc>::getCounter()
{
    return s_opCounter;
}

template<typename Alloc>
OpTrackerAllocatorFirst<Alloc>::OpTrackerAllocatorFirst()
    : OpTracker(s_opCounter)
    , m_allocator()
{
    s_opCounter.addDefaultConstructor();
}

template<typename Alloc>
OpTrackerAllocatorFirst<Alloc>::OpTrackerAllocatorFirst(allocator_type const& alloc)
    : OpTracker(s_opCounter)
    , m_allocator(alloc)
{
    s_opCounter.addCopyConstructorAlloc();
}

template<typename Alloc>
OpTrackerAllocatorFirst<Alloc>::OpTrackerAllocatorFirst(std::allocator_arg_t,
                                                        allocator_type const& alloc,
                                                        value_type const&     value)
    : OpTracker(s_opCounter, value)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorFirst();
}

template<typename Alloc>
OpTrackerAllocatorFirst<Alloc>::OpTrackerAllocatorFirst(std::allocator_arg_t,
                                                        allocator_type const& alloc,
                                                        value_type const&     value,
                                                        int                   extra)
    : OpTracker(s_opCounter, value + extra)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorFirst();
}

} // namespace pw::test

namespace std {
template<typename Alloc>
struct uses_allocator<pw::test::OpTrackerAllocatorFirst<Alloc>, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_FIRST_H */