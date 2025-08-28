#ifndef INCLUDED_PW_TEST_ALLOCATOR_ONLY_TYPE_H
#define INCLUDED_PW_TEST_ALLOCATOR_ONLY_TYPE_H

#include <memory>
#include <pw/impl/allocator.h>
#include <test_optracker.h>

namespace pw::test {

struct AllocatorOnlyType : public OpTracker
{
    static OpCounter getCounter();

    AllocatorOnlyType() = delete;
    template<class Alloc>
    explicit AllocatorOnlyType(const Alloc& alloc);

private:
    static OpCounter s_opCounter;
};

template<typename Alloc>
AllocatorOnlyType::AllocatorOnlyType(const Alloc&)
    : OpTracker(s_opCounter)
{
    s_opCounter.addOtherConstructor().addAllocatorOnly();
}

} // namespace pw::test

namespace std {
template<class Alloc>
struct uses_allocator<pw::test::AllocatorOnlyType, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_ALLOCATOR_ONLY_TYPE_H */