#include <test_optracker_allocator_none.h>

namespace pw::test {

OpCounter OpTrackerAllocatorNone::s_opCounter;

OpCounter
OpTrackerAllocatorNone::getCounter()
{
    return s_opCounter;
}

OpTrackerAllocatorNone::OpTrackerAllocatorNone()
    : OpTracker(s_opCounter)
{
    s_opCounter.addDefaultConstructor().addNoAllocator();
}

OpTrackerAllocatorNone::OpTrackerAllocatorNone(value_type const& value)
    : OpTracker(s_opCounter, value)
{
    s_opCounter.addOtherConstructor().addNoAllocator();
}

OpTrackerAllocatorNone::OpTrackerAllocatorNone(value_type const& value, int extra)
    : OpTracker(s_opCounter, value + extra)
{
    s_opCounter.addOtherConstructor().addNoAllocator();
}

} // namespace pw::test