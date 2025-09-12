#ifndef INCLUDED_PW_TEST_OPTRACKER_EMPLACEMOVECONSTRUCTIBLE_H
#define INCLUDED_PW_TEST_OPTRACKER_EMPLACEMOVECONSTRUCTIBLE_H

#include <compare>
#include <pw/impl/move.h>
#include <test_optracker.h>

namespace pw::test {

struct OpTrackerEmplaceMoveConstructible : public OpTracker
{
    static OpCounter& getCounter();
    OpTrackerEmplaceMoveConstructible() = delete;

    OpTrackerEmplaceMoveConstructible(int value, int value2)
        : OpTracker(s_opCounter, value)
        , m_value2(value2)
    {
    }

    OpTrackerEmplaceMoveConstructible(OpTrackerEmplaceMoveConstructible const& copy)
        : OpTracker(copy)
        , m_value2(copy.m_value2)
    {
    }

    OpTrackerEmplaceMoveConstructible(OpTrackerEmplaceMoveConstructible&& move) noexcept
        : OpTracker(pw::move(move))
        , m_value2(pw::move(move.m_value2))
    {
    }

    ~OpTrackerEmplaceMoveConstructible() { }

    OpTrackerEmplaceMoveConstructible& operator=(OpTrackerEmplaceMoveConstructible const& op2)
    {
        OpTracker::operator=(op2);
        m_value2 = op2.m_value2;
        return *this;
    }

    int                                value2() const { return m_value2; }

    OpTrackerEmplaceMoveConstructible& setValue2(int value)
    {
        m_value2 = value;
        return *this;
    }

    std::strong_ordering operator<=>(OpTrackerEmplaceMoveConstructible const& op2) const;

private:
    int              m_value2 = 1;
    static OpCounter s_opCounter;
};

} // namespace pw::test
#endif /*  INCLUDED_PW_TEST_OPTRACKER_EMPLACEMOVECONSTRUCTIBLE_H */
