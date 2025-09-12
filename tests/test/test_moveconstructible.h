#ifndef INCLUDED_PW_TEST_MOVECONSTRUCTIBLE_H
#define INCLUDED_PW_TEST_MOVECONSTRUCTIBLE_H

#include <compare>
#include <test_optracker.h>

namespace pw::test {

/**
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
struct MoveConstructible : public OpTracker
{
    static OpCounter getCounter();

    MoveConstructible();
    explicit MoveConstructible(value_type const& value);
    MoveConstructible(MoveConstructible const& copy);
    MoveConstructible(MoveConstructible&& move) noexcept;

    MoveConstructible&   operator=(MoveConstructible&& move) noexcept;
    MoveConstructible&   operator=(MoveConstructible const& copy);
    bool                 operator==(MoveConstructible const& op2) const;
    bool                 operator!=(MoveConstructible const& op2) const;
    std::strong_ordering operator<=>(MoveConstructible const& op2) const;

private:
    static OpCounter s_opCounter;
};

bool permute(MoveConstructible& value, int depth);

} // namespace pw::test
#endif /*  INCLUDED_PW_TEST_MOVECONSTRUCTIBLE_H */
