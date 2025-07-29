#ifndef INCLUDED_TEST_COPYCONSTRUCTIBLE_H
#define INCLUDED_TEST_COPYCONSTRUCTIBLE_H

#include <test_optracker.h>
#include <compare>

namespace pw::test {
/**
 * The type T satisfies @code CopyConstructible if
 * The type T satisfies MoveConstructible, and
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
 * @code
 * T(v)	        // The value of T(v) is equivalent to the value of v.
 * @endcode
 * The value of v is unchanged.
 * 
 */
struct CopyConstructible : public OpTracker
{
    static OpCounter getCounter();

    CopyConstructible(OpTracker::value_type const& value);
    CopyConstructible(CopyConstructible const& copy);
    CopyConstructible(CopyConstructible&& move) noexcept;
    ~CopyConstructible();

    CopyConstructible& operator=(CopyConstructible const& copy);
    CopyConstructible& operator=(CopyConstructible&& move);
    using OpTracker::operator==;
    std::strong_ordering operator<=>(CopyConstructible const& op2) const;
    // using OpTracker::operator!=;
    // using OpTracker::operator<;

private:
    static OpCounter s_opCounter;
};

bool permute(CopyConstructible& value, int depth);
} // namespace pw::test

#endif /*  INCLUDED_TEST_COPYCONSTRUCTIBLE_H */
