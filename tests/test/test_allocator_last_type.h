#ifndef INCLUDED_PW_TEST_ALLOCATOR_LAST_TYPE_H
#define INCLUDED_PW_TEST_ALLOCATOR_LAST_TYPE_H

#include <memory>
#include <test_optracker.h>

namespace pw::test {

template<typename Alloc>
struct AllocatorLastType : public OpTracker
{
    using allocator_type = Alloc;

    static OpCounter getCounter();

    AllocatorLastType();
    explicit AllocatorLastType(allocator_type const& alloc);
    AllocatorLastType(value_type const& value, allocator_type const& alloc);
    AllocatorLastType(value_type const& value, int extra, allocator_type const& alloc);

private:
    static OpCounter s_opCounter;
    allocator_type   m_allocator;
};

template<typename Alloc>
OpCounter AllocatorLastType<Alloc>::s_opCounter;

template<typename Alloc>
OpCounter
AllocatorLastType<Alloc>::getCounter()
{
    return s_opCounter;
}

template<typename Alloc>
AllocatorLastType<Alloc>::AllocatorLastType()
    : OpTracker(s_opCounter)
    , m_allocator()
{
    s_opCounter.addDefaultConstructor().addNoAllocator();
}

template<typename Alloc>
AllocatorLastType<Alloc>::AllocatorLastType(allocator_type const& alloc)
    : OpTracker(s_opCounter)
    , m_allocator(alloc)
{
    s_opCounter.addDefaultConstructor().addAllocatorLast();
}

template<typename Alloc>
AllocatorLastType<Alloc>::AllocatorLastType(value_type const& value, allocator_type const& alloc)
    : OpTracker(s_opCounter, value)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorLast();
}

template<typename Alloc>
AllocatorLastType<Alloc>::AllocatorLastType(value_type const& value, int extra, allocator_type const& alloc)
    : OpTracker(s_opCounter, value + extra)
    , m_allocator(alloc)
{
    s_opCounter.addOtherConstructor().addAllocatorLast();
}

} // namespace pw::test

namespace std {
template<typename Alloc>
struct uses_allocator<pw::test::AllocatorLastType<Alloc>, Alloc> : std::true_type
{
};
} // namespace std

#endif /* INCLUDED_PW_TEST_ALLOCATOR_LAST_TYPE_H */