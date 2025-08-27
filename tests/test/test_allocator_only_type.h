#ifndef INCLUDED_PW_TEST_ALLOCATOR_ONLY_TYPE_H
#define INCLUDED_PW_TEST_ALLOCATOR_ONLY_TYPE_H

#include <memory>
#include <pw/impl/allocator.h>
#include <test_optracker.h>

namespace pw::test {

template<class Alloc = allocator<int>>
struct AllocatorOnlyType : public OpTracker
{
    using allocator_type = Alloc;

    static OpCounter getCounter();

    AllocatorOnlyType() = delete;
    explicit AllocatorOnlyType(const allocator_type& alloc = allocator_type {});

private:
    static OpCounter s_opCounter;
    allocator_type   m_allocator;
};

template<typename Alloc>
OpCounter AllocatorOnlyType<Alloc>::s_opCounter;

template<typename Alloc>
OpCounter
AllocatorOnlyType<Alloc>::getCounter()
{
    return s_opCounter;
}

template<typename Alloc>
AllocatorOnlyType<Alloc>::AllocatorOnlyType(const Alloc& alloc)
    : OpTracker(s_opCounter)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorOnly();
}

} // namespace pw::test

namespace std {
template<typename Alloc>
struct uses_allocator<pw::test::AllocatorOnlyType<Alloc>, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_ALLOCATOR_ONLY_TYPE_H */