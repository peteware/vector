#include <pw/impl/move.h>

#include <test_optracker_copyconstructible.h>
#include <test_permute.h>

namespace pw::test {

OpCounter OpTrackerCopyConstructible::s_opCounter;

OpCounter
OpTrackerCopyConstructible::getCounter()
{
    return s_opCounter;
}

OpTrackerCopyConstructible::OpTrackerCopyConstructible(value_type const& value)
    : OpTracker(s_opCounter, value)
{
}

OpTrackerCopyConstructible::OpTrackerCopyConstructible(OpTrackerCopyConstructible const& copy)
    : OpTracker(copy)
{
}

OpTrackerCopyConstructible::OpTrackerCopyConstructible(OpTrackerCopyConstructible&& move) noexcept
    : OpTracker(pw::move(move))
{
}

OpTrackerCopyConstructible::~OpTrackerCopyConstructible()
{
}

OpTrackerCopyConstructible&
OpTrackerCopyConstructible::operator=(OpTrackerCopyConstructible const& copy)
{
    OpTracker::operator=(copy);
    return *this;
}

OpTrackerCopyConstructible&
OpTrackerCopyConstructible::operator=(OpTrackerCopyConstructible&& move) noexcept
{
    OpTracker::operator=(pw::move(move));
    return *this;
}

std::strong_ordering
OpTrackerCopyConstructible::operator<=>(OpTrackerCopyConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

bool
permute(OpTrackerCopyConstructible& value, int depth)
{
    if (int val = value.value(); permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}

} // namespace pw::test
