#include <test_defaultcopyconstructible.h>

#include <test_permute.h>

namespace pw { namespace test {

OpCounter DefaultCopyConstructible::s_opCounter;

OpCounter
DefaultCopyConstructible::getCounter()
{
    return s_opCounter;
}

bool
permute(DefaultCopyConstructible& value, int depth)
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
