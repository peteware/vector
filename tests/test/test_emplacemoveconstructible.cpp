#include <test_emplacemoveconstructible.h>

namespace pw::test {
OpCounter EmplaceMoveConstructible::s_opCounter;

OpCounter
EmplaceMoveConstructible::getCounter()
{
    return s_opCounter;
}

std::strong_ordering
EmplaceMoveConstructible::operator<=>(EmplaceMoveConstructible const& op2) const
{
    return OpTracker::operator<=>(op2);
}

} // namespace pw::test
