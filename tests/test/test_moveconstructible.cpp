#include <pw/impl/move.h>

#include <test_moveconstructible.h>
#include <test_permute.h>

namespace pw::test {
OpCounter MoveConstructible::s_opCounter;

OpCounter
MoveConstructible::getCounter()
{
    return s_opCounter;
}

MoveConstructible::MoveConstructible()
    : OpTracker(s_opCounter)
{
}

MoveConstructible::MoveConstructible(value_type const& value)
    : OpTracker(s_opCounter, value)
{
}

MoveConstructible::MoveConstructible(MoveConstructible const& copy)
    : OpTracker(copy)
{
}

MoveConstructible::MoveConstructible(MoveConstructible&& move) noexcept
    : OpTracker(pw::move(move))
{
}

MoveConstructible&
MoveConstructible::operator=(MoveConstructible&& move) noexcept
{
    OpTracker::operator=(pw::move(move));
    return *this;
}

MoveConstructible&
MoveConstructible::operator=(MoveConstructible const& copy)
{
    OpTracker::operator=(copy);
    return *this;
}

bool
MoveConstructible::operator==(MoveConstructible const& op2) const
{
    return OpTracker::operator==(op2);
}

bool
MoveConstructible::operator!=(MoveConstructible const& op2) const
{
    return OpTracker::operator!=(op2);
}

std::strong_ordering
MoveConstructible::operator<=>(MoveConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

bool
permute(MoveConstructible& value, int depth)
{
    if (int val = value.value(); permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}
} // namespace pw::test
