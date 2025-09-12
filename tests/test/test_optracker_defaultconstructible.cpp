#include <test_optracker_defaultconstructible.h>

namespace pw::test {

OpCounter OpTrackerDefaultConstructible::s_opCounter;

OpCounter
OpTrackerDefaultConstructible::getCounter()
{
    return s_opCounter;
}

std::strong_ordering
OpTrackerDefaultConstructible::operator<=>(OpTrackerDefaultConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

} // namespace pw::test
