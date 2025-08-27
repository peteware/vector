#include <test_no_allocator_type.h>

namespace pw::test {

OpCounter NoAllocatorType::s_opCounter;

OpCounter
NoAllocatorType::getCounter()
{
    return s_opCounter;
}

NoAllocatorType::NoAllocatorType()
    : OpTracker(s_opCounter)
{
    s_opCounter.addDefaultConstructor().addNoAllocator();
}

NoAllocatorType::NoAllocatorType(value_type const& value)
    : OpTracker(s_opCounter, value)
{
    s_opCounter.addOtherConstructor().addNoAllocator();
}

NoAllocatorType::NoAllocatorType(value_type const& value, int extra)
    : OpTracker(s_opCounter, value + extra)
{
    s_opCounter.addOtherConstructor().addNoAllocator();
}

} // namespace pw::test