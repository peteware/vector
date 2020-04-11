#ifndef INCLUDED_OPTRACKER_H
#define INCLUDED_OPTRACKER_H

#include "opcounter.h"
#include <pw/impl/move.h>

struct OpTracker
{
protected:
    OpTracker(OpCounter& opCounter, int value);
    OpTracker(OpTracker const& copy);
    OpTracker(OpTracker&& move) noexcept;
    ~OpTracker();

public:
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

struct DefaultConstructible : public OpTracker
{
    static OpCounter getCounter();

    DefaultConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

private:
    static OpCounter s_opCounter;
};

struct CopyConstructible : public OpTracker
{
    static OpCounter getCounter();

    CopyConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

    CopyConstructible(CopyConstructible const& copy)
        : OpTracker(copy)
    {
    }

    CopyConstructible(CopyConstructible&& move) noexcept
        : OpTracker(pw::move(move))
    {
    }

    ~CopyConstructible()
    {
    }

    CopyConstructible& operator=(CopyConstructible const& copy)
    {
        OpTracker::operator=(copy);
        return *this;
    }

    CopyConstructible& operator=(CopyConstructible&& move)
    {
        OpTracker::operator=(pw::move(move));
        return *this;
    }

    using OpTracker::operator==;
    // using OpTracker::operator!=;
    // using OpTracker::operator<;

private:
    static OpCounter s_opCounter;
};

struct MoveConstructible : OpTracker
{
    static OpCounter getCounter();
    MoveConstructible(int value = 0)
        : OpTracker(s_opCounter, value)
    {
    }

    MoveConstructible(MoveConstructible&& move)
        : OpTracker(pw::move(move))
    {
    }

    MoveConstructible& operator=(MoveConstructible&& move)
    {
        OpTracker::operator=(pw::move(move));
        return *this;
    }

private:
    static OpCounter s_opCounter;
};

#endif /*  INCLUDED_OPTRACKER_H */
