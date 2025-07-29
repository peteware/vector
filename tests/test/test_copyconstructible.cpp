#include <test_copyconstructible.h>

#include <test_permute.h>

namespace pw::test {

OpCounter CopyConstructible::s_opCounter;

OpCounter
CopyConstructible::getCounter()
{
    return s_opCounter;
}

CopyConstructible::CopyConstructible(OpTracker::value_type const& value)
    : OpTracker(s_opCounter, value)
{
}

CopyConstructible::CopyConstructible(CopyConstructible const& copy)
    : OpTracker(copy)
{
}

CopyConstructible::CopyConstructible(CopyConstructible&& move) noexcept
    : OpTracker(pw::move(move))
{
}

CopyConstructible::~CopyConstructible()
{
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible const& copy)
{
    OpTracker::operator=(copy);
    return *this;
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible&& move)
{
    OpTracker::operator=(pw::move(move));
    return *this;
}

std::strong_ordering
CopyConstructible::operator<=>(CopyConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

bool
permute(CopyConstructible& value, int depth)
{
    int val = value.value();
    if (permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}

} // namespace pw::test
