#ifndef INCLUDED_PW_TEST_OPTRACKER_ALLOCATORONLY_H
#define INCLUDED_PW_TEST_OPTRACKER_ALLOCATORONLY_H

#include <memory>
#include <pw/impl/allocator.h>
#include <test_optracker.h>

namespace pw::test {

struct OpTrackerAllocatorOnly : public OpTracker
{
    static OpCounter getCounter();

    OpTrackerAllocatorOnly() = delete;
    template<class Alloc>
    explicit OpTrackerAllocatorOnly(Alloc const& alloc);

private:
    static OpCounter s_opCounter;
};

template<typename Alloc>
OpTrackerAllocatorOnly::OpTrackerAllocatorOnly(Alloc const&)
    : OpTracker(s_opCounter)
{
    s_opCounter.addOtherConstructor().addAllocatorOnly();
}

} // namespace pw::test

namespace std {
template<class Alloc>
struct uses_allocator<pw::test::OpTrackerAllocatorOnly, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_OPTRACKER_ALLOCATORONLY_H */