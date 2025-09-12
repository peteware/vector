#include <test_optracker_noallocator.h>

namespace pw::test {

OpCounter OpTrackerNoAllocator::s_opCounter;

OpCounter
OpTrackerNoAllocator::getCounter()
{
    return s_opCounter;
}

OpTrackerNoAllocator::OpTrackerNoAllocator()
    : OpTracker(s_opCounter)
{
    s_opCounter.addDefaultConstructor().addNoAllocator();
}

OpTrackerNoAllocator::OpTrackerNoAllocator(value_type const& value)
    : OpTracker(s_opCounter, value)
{
    s_opCounter.addOtherConstructor().addNoAllocator();
}

OpTrackerNoAllocator::OpTrackerNoAllocator(value_type const& value, int extra)
    : OpTracker(s_opCounter, value + extra)
{
    s_opCounter.addOtherConstructor().addNoAllocator();
}

} // namespace pw::test