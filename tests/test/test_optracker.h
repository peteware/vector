#ifndef INCLUDED_TEST_OPTRACKER_H
#define INCLUDED_TEST_OPTRACKER_H

#include <pw/impl/move.h>
#include <test_opcounter.h>

#include <iosfwd>

namespace pw { namespace test {

struct OpTracker
{
protected:
    OpTracker() = delete;
    OpTracker(OpCounter& opCounter); // default constructor for derived classes
    OpTracker(OpCounter& opCounter, int value);
    OpTracker(OpTracker const& copy);
    OpTracker(OpTracker&& move) noexcept;
    ~OpTracker();

public:
    using value_type = int;
    int        value() const;
    OpTracker& setValue(int value);

protected:
    OpTracker& operator=(OpTracker const& copy);
    OpTracker& operator=(OpTracker&& copy);
    bool       operator==(OpTracker const& op2) const;
    bool       operator!=(OpTracker const& op2) const;
    bool       operator<(OpTracker const& op2) const;

private:
    OpCounter& m_opCounter;
    int        m_value = 0;
};

bool permute(OpTracker& value, int depth);
std::ostream& operator<<(std::ostream& out, OpTracker const& op2);
}} // namespace pw::test

#endif /*  INCLUDED_TEST_OPTRACKER_H */
