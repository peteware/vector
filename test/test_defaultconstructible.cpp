#include <test_optracker.h>
#include <test_permute.h>


namespace pw { namespace test {

OpCounter DefaultConstructible::s_opCounter;

OpCounter
DefaultConstructible::getCounter()
{
    return s_opCounter;
}

}} // namespace pw::test
