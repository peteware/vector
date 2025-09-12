#include <test_optracker_emplacemoveconstructible.h>

namespace pw::test {
OpCounter OpTrackerEmplaceMoveConstructible::s_opCounter;

OpCounter&
OpTrackerEmplaceMoveConstructible::getCounter()
{
    return s_opCounter;
}

std::strong_ordering
OpTrackerEmplaceMoveConstructible::operator<=>(OpTrackerEmplaceMoveConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

} // namespace pw::test
