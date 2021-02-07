#include <test_emplacemoveconstructible.h>

namespace pw { namespace test {
OpCounter EmplaceMoveConstructible::s_opCounter;

OpCounter
EmplaceMoveConstructible::getCounter()
{
    return s_opCounter;
}

}} // namespace pw::test
