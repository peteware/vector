#ifndef INCLUDED_PW_TEST_OPTRACKER_MOVECONSTRUCTIBLE_H
#define INCLUDED_PW_TEST_OPTRACKER_MOVECONSTRUCTIBLE_H

#include <compare>
#include <test_optracker.h>

namespace pw::test {

/**
 * A copy and move constructible OpTracker with OpCounter
 *
 * Also allows copy and move assignment
 *
 * Given
 * - @c rv, and rvalue expression of type T
 * - @c u, an arbitrary identifier
 * Then
 * @code
 * T u = rv;
 * T u(rv);
 * @endcode
 *
 * Note: copy constructible also satisfies this
 */
struct OpTrackerMoveConstructible : public OpTracker
{
    static OpCounter& getCounter();

    OpTrackerMoveConstructible();
    explicit OpTrackerMoveConstructible(value_type const& value);
    OpTrackerMoveConstructible(OpTrackerMoveConstructible const& copy);
    OpTrackerMoveConstructible(OpTrackerMoveConstructible&& move) noexcept;

    OpTrackerMoveConstructible& operator=(OpTrackerMoveConstructible&& move) noexcept;
    OpTrackerMoveConstructible& operator=(OpTrackerMoveConstructible const& copy);
    bool                        operator==(OpTrackerMoveConstructible const& op2) const;
    bool                        operator!=(OpTrackerMoveConstructible const& op2) const;
    std::strong_ordering        operator<=>(OpTrackerMoveConstructible const& op2) const;

private:
    static OpCounter s_opCounter;
};

bool permute(OpTrackerMoveConstructible& value, int depth);

} // namespace pw::test
#endif /*  INCLUDED_PW_TEST_OPTRACKER_MOVECONSTRUCTIBLE_H */
