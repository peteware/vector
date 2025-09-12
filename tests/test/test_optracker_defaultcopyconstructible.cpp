#include <test_optracker_defaultcopyconstructible.h>
#include <test_permute.h>

#include <iostream>

namespace pw::test {

OpCounter OpTrackerDefaultCopyConstructible::s_opCounter;

std::strong_ordering
OpTrackerDefaultCopyConstructible::operator<=>(OpTrackerDefaultCopyConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

bool
permute(OpTrackerDefaultCopyConstructible& value, int depth)
{
    if (int val = value.value(); permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}

} // namespace pw::test
