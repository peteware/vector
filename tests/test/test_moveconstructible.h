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
    using value_type = OpTracker::value_type;
    static OpCounter getCounter();

    MoveConstructible();
    MoveConstructible(value_type const& value);
    MoveConstructible(MoveConstructible const& copy);
    MoveConstructible(MoveConstructible&& move);

    MoveConstructible& operator=(MoveConstructible&& move);
    MoveConstructible& operator=(MoveConstructible const& copy);
    bool               operator==(MoveConstructible const& op2) const;
    bool               operator!=(MoveConstructible const& op2) const;

private:
    static OpCounter s_opCounter;
};

bool permute(MoveConstructible& value, int depth);

}} // namespace pw::test
#endif /*  INCLUDED_TEST_MOVECONSTRUCTIBLE_H */
