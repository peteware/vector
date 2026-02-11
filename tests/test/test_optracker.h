#ifndef INCLUDED_PW_TEST_OPTRACKER_H
#define INCLUDED_PW_TEST_OPTRACKER_H

#include <test_opcounter.h>

#include <compare>
#include <iosfwd>

namespace pw::test {

/**
 * Base class for a value type that counts operations
 *
 * See OpCounter for the action operations but a sampling:
 * - Default Constructor
 * - Copy Constructor
 * - Assignment operator
 * - Destructor
 * - Comparison operators
 * - etc
 *
 * The derived classes are used to check that a container implementation
 * is calling the correct operators the correct number of times.  For example,
 * with N elements the assignment operator should call the destructor
 * N times and the copy constructor M times.
 */
struct OpTracker
{
    using value_type = int;
    OpTracker()      = delete;
    [[nodiscard]] value_type value() const;
    OpTracker&               setValue(value_type const& value);
    [[nodiscard]] OpCounter  opCounter() const;
    OpTracker&               reset();

protected:
    explicit OpTracker(OpCounter& opCounter); // default constructor for derived classes
    OpTracker(OpCounter& opCounter, value_type const& value);
    OpTracker(OpTracker const& copy);
    OpTracker(OpTracker&& move) noexcept;
    ~OpTracker();

    OpTracker&           operator=(OpTracker const& copy);
    OpTracker&           operator=(OpTracker&& move) noexcept;
    bool                 operator==(OpTracker const& op2) const;
    bool                 operator!=(OpTracker const& op2) const;
    bool                 operator<(OpTracker const& op2) const;
    std::strong_ordering operator<=>(OpTracker const& op2) const;

private:
    OpCounter& m_opCounter;
    value_type m_value = 0;
};

bool          permute(OpTracker& value, int depth);
std::ostream& operator<<(std::ostream& out, OpTracker const& op2);
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_OPTRACKER_H */
