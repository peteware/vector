#ifndef INCLUDED_TEST_EMPLACEMOVECONSTRUCTIBLE_H
#define INCLUDED_TEST_EMPLACEMOVECONSTRUCTIBLE_H

#include <pw/impl/move.h>
#include <test_optracker.h>

namespace pw::test {

struct EmplaceMoveConstructible : public OpTracker
{
    static OpCounter getCounter();
    EmplaceMoveConstructible() = delete;

    EmplaceMoveConstructible(int value, int value2)
        : OpTracker(s_opCounter, value)
        , m_value2(value2)
    {
    }

    EmplaceMoveConstructible(EmplaceMoveConstructible const& copy)
        : OpTracker(copy)
        , m_value2(copy.m_value2)
    {
    }

    EmplaceMoveConstructible(EmplaceMoveConstructible&& move)
        : OpTracker(pw::move(move))
        , m_value2(pw::move(move.m_value2))
    {
    }

    ~EmplaceMoveConstructible() { }

    EmplaceMoveConstructible& operator=(EmplaceMoveConstructible const& op2)
    {
        OpTracker::operator=(op2);
        m_value2 = op2.m_value2;
        return *this;
    }

    int                       value2() const { return m_value2; }

    EmplaceMoveConstructible& setValue2(int value)
    {
        m_value2 = value;
        return *this;
    }

private:
    int              m_value2 = 1;
    static OpCounter s_opCounter;
};

} // namespace pw::test
#endif /*  INCLUDED_TEST_EMPLACEMOVECONSTRUCTIBLE_H */
