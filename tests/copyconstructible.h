#ifndef INCLUDED_COPYCONSTRUCTIBLE_H
#define INCLUDED_COPYCONSTRUCTIBLE_H

#include "opcounter.h"

struct OpTracker
{
    int        value() const;
    OpTracker& setValue(int value);

protected:
    OpTracker(OpCounter& opCounter, int value);
    OpTracker(OpTracker const& copy);
    OpTracker(OpTracker&& move);
    ~OpTracker();

    OpTracker& operator=(OpTracker const& copy);
    OpTracker& operator=(OpTracker&& copy);
    bool       operator==(OpTracker const& op2) const;
    bool       operator!=(OpTracker const& op2) const;
    bool       operator<(OpTracker const& op2) const;

private:
    OpCounter& m_opCounter;
    int        m_value = 0;
};

struct DefaultConstructible : public OpTracker
{
    DefaultConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

private:
    static s_opCounter;
};

struct CopyConstructible : public OpTracker
{
    static OpCounter getCounter();

    CopyConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

    CopyConstructible(CopyConstructible const& copy);
    CopyConstructible(CopyConstructible&& copy) noexcept;
    ~CopyConstructible();

private:
    static OpCounter s_opCounter;
};

bool permute(OpTracker& value, int depth);

#endif /*  INCLUDED_COPYCONSTRUCTIBLE_H */
