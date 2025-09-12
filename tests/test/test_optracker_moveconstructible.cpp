#include <pw/impl/move.h>

#include <test_optracker_moveconstructible.h>
#include <test_permute.h>

namespace pw::test {
OpCounter OpTrackerMoveConstructible::s_opCounter;

OpCounter
OpTrackerMoveConstructible::getCounter()
{
    return s_opCounter;
}

OpTrackerMoveConstructible::OpTrackerMoveConstructible()
    : OpTracker(s_opCounter)
{
}

OpTrackerMoveConstructible::OpTrackerMoveConstructible(value_type const& value)
    : OpTracker(s_opCounter, value)
{
}

OpTrackerMoveConstructible::OpTrackerMoveConstructible(OpTrackerMoveConstructible const& copy)
    : OpTracker(copy)
{
}

OpTrackerMoveConstructible::OpTrackerMoveConstructible(OpTrackerMoveConstructible&& move) noexcept
    : OpTracker(pw::move(move))
{
}

OpTrackerMoveConstructible&
OpTrackerMoveConstructible::operator=(OpTrackerMoveConstructible&& move) noexcept
{
    OpTracker::operator=(pw::move(move));
    return *this;
}

OpTrackerMoveConstructible&
OpTrackerMoveConstructible::operator=(OpTrackerMoveConstructible const& copy)
{
    OpTracker::operator=(copy);
    return *this;
}

bool
OpTrackerMoveConstructible::operator==(OpTrackerMoveConstructible const& op2) const
{
    return OpTracker::operator==(op2);
}

bool
OpTrackerMoveConstructible::operator!=(OpTrackerMoveConstructible const& op2) const
{
    return OpTracker::operator!=(op2);
}

std::strong_ordering
OpTrackerMoveConstructible::operator<=>(OpTrackerMoveConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

bool
permute(OpTrackerMoveConstructible& value, int depth)
{
    if (int val = value.value(); permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}
} // namespace pw::test
