#ifndef INCLUDED_PW_TEST_OPTRACKER_COPYCONSTRUCTIBLE_H
#define INCLUDED_PW_TEST_OPTRACKER_COPYCONSTRUCTIBLE_H

#include <compare>
#include <test_optracker.h>

namespace pw::test {
/**
 * The type T satisfies @c CopyConstructible if
 * The type T satisfies OpTrackerMoveConstructible, and
 * Given:
 * - @c v, an lvalue expression of type T or const T or an rvalue expression of type const T
 * - @c u, an arbitrary identifier
 *
 * The following expressions must be valid and have their specified effects
 * Expression	Post-conditions
 * @code
 * T u = v;	// The value of u is equivalent to the value of v.
 * @endcode
 * The value of v is unchanged
 *
 * @code
 * v == T(v);	        // The value of T(v) is equivalent to the value of v.
 * @endcode
 *
 * The value of v is unchanged.
 */
struct OpTrackerCopyConstructible : public OpTracker
{
    static OpCounter getCounter();

    // ReSharper disable once CppNonExplicitConvertingConstructor
    OpTrackerCopyConstructible(value_type const& value);
    OpTrackerCopyConstructible(OpTrackerCopyConstructible const& copy);
    OpTrackerCopyConstructible(OpTrackerCopyConstructible&& move) noexcept;
    ~OpTrackerCopyConstructible();

    OpTrackerCopyConstructible& operator=(OpTrackerCopyConstructible const& copy);
    OpTrackerCopyConstructible& operator=(OpTrackerCopyConstructible&& move) noexcept;
    using OpTracker::operator==;
    std::strong_ordering operator<=>(OpTrackerCopyConstructible const& op2) const;
    // using OpTracker::operator!=;
    // using OpTracker::operator<;

private:
    static OpCounter s_opCounter;
};

bool permute(OpTrackerCopyConstructible& value, int depth);
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_OPTRACKER_COPYCONSTRUCTIBLE_H */
