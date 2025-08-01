#ifndef INCLUDED_TEST_OPTRACKER_H
#define INCLUDED_TEST_OPTRACKER_H

#include <pw/impl/move.h>
#include <test_opcounter.h>

#include <iosfwd>

namespace pw::test {

struct OpTracker
{
public:
    using value_type = int;
    value_type value() const;
    OpTracker& setValue(value_type const& value);

protected:
    OpTracker() = delete;
    OpTracker(OpCounter& opCounter); // default constructor for derived classes
    OpTracker(OpCounter& opCounter, value_type const& value);
    OpTracker(OpTracker const& copy);
    OpTracker(OpTracker&& move) noexcept;
    ~OpTracker();

    OpTracker& operator=(OpTracker const& copy);
    OpTracker& operator=(OpTracker&& move);
    bool       operator==(OpTracker const& op2) const;
    bool       operator!=(OpTracker const& op2) const;
    bool       operator<(OpTracker const& op2) const;

private:
    OpCounter& m_opCounter;
    value_type m_value = 0;
};

bool          permute(OpTracker& value, int depth);
std::ostream& operator<<(std::ostream& out, OpTracker const& op2);
} // namespace pw::test

#endif /*  INCLUDED_TEST_OPTRACKER_H */
