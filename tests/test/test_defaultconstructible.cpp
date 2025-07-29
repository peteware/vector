#include <test_defaultconstructible.h>

namespace pw::test {

OpCounter DefaultConstructible::s_opCounter;

OpCounter
DefaultConstructible::getCounter()
{
    return s_opCounter;
}

std::strong_ordering
DefaultConstructible::operator<=>(DefaultConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

} // namespace pw::test
