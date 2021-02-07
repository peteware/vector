#include <test_moveconstructible.h>

namespace pw { namespace test {
OpCounter MoveConstructible::s_opCounter;

OpCounter
MoveConstructible::getCounter()
{
    return s_opCounter;
}

}} // namespace pw::test
