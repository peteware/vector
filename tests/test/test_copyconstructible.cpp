#include <test_copyconstructible.h>

#include <test_permute.h>

namespace pw { namespace test {

OpCounter CopyConstructible::s_opCounter;

OpCounter
CopyConstructible::getCounter()
{
    return s_opCounter;
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

}} // namespace pw::test
