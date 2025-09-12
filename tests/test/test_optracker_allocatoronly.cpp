#include <test_optracker_allocatoronly.h>

namespace pw::test {

OpCounter OpTrackerAllocatorOnly::s_opCounter;

OpCounter
OpTrackerAllocatorOnly::getCounter()
{
    return s_opCounter;
}

} // namespace pw::test
