#ifndef INCLUDED_TEST_MOVECONSTRUCTIBLE_H
#define INCLUDED_TEST_MOVECONSTRUCTIBLE_H

#include <test_optracker.h>

namespace pw { namespace test {

/**
 * Given
 * - @c rv, and rvalue expression of type T
 * - @c u, an abritray identifier
 * Then
 * @code
 * T u = rv;
 * T u(rv);
 * @endcode
 *
 * Note: copy constructible also satisifies this
 */
struct MoveConstructible : public OpTracker
{
    static OpCounter getCounter();

    MoveConstructible()
        : OpTracker(s_opCounter)
    {
    }

    MoveConstructible(int value)
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
