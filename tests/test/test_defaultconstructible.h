#ifndef INCLUDED_TEST_DEFAULTCONSTRUCTIBLE_H
#define INCLUDED_TEST_DEFAULTCONSTRUCTIBLE_H

#include <test_optracker.h>

namespace pw { namespace test {

/*
 * Needed type requirements
 * - Erasable (everything)
 *   - MoveInsertable (reserve(), shrink_to_fit())
 *     - EmplaceConstructible (emplace_back())
 *       - MoveAssignable (emplace())
 *   - MoveAssignable (erase())
 *     - MoveInsertable (insert())
 *   - CopyInsertable
 *     - CopyAssignable (insert())
 *   - DefaultInsertable
 *   - Swappable, MoveAssignable, MoveConstructible and MoveInsertable 
 *   - EmplaceConstructible
 *   - CopyAssignable and CopyInsertable
 */

struct DefaultConstructible : public OpTracker
{
    static OpCounter getCounter();

    DefaultConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

private:
    static OpCounter s_opCounter;
};

}} // namespace pw::test

#endif /* INCLUDED_TEST_DEFAULTCONSTRUCTIBLE_H */
