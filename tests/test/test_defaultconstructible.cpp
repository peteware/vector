#include <test_defaultconstructible.h>

namespace pw::test {

OpCounter DefaultConstructible::s_opCounter;

OpCounter
DefaultConstructible::getCounter()
{
    return s_opCounter;
}

} // namespace pw::test
