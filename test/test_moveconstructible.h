#ifndef INCLUDED_TEST_MOVECONSTRUCTIBLE_H
#define INCLUDED_TEST_MOVECONSTRUCTIBLE_H

#include <test_optracker.h>

namespace pw { namespace test {

struct MoveConstructible : public OpTracker
{
    static OpCounter getCounter();
    MoveConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

    MoveConstructible(MoveConstructible&& move)
        : OpTracker(pw::move(move))
    {
    }

    MoveConstructible& operator=(MoveConstructible&& move)
    {
        OpTracker::operator=(pw::move(move));
        return *this;
    }

private:
    static OpCounter s_opCounter;
};

}} // namespace pw::test
#endif /*  INCLUDED_TEST_MOVECONSTRUCTIBLE_H */
