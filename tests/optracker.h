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

bool permute(CopyConstructible& value, int depth);

struct MoveConstructible : public OpTracker
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

struct EmplaceMoveConstructible : public OpTracker
{
    static OpCounter getCounter();

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
    {
    }

    ~EmplaceMoveConstructible()
    {
    }

    EmplaceMoveConstructible& operator=(EmplaceMoveConstructible const& op2)
    {
        OpTracker::operator=(op2);
        m_value2           = op2.m_value2;
        return *this;
    }

    int value2() const
    {
        return m_value2;
    }
    EmplaceMoveConstructible& setValue2(int value)
    {
        m_value2 = value;
        return *this;
    }

private:
    int              m_value2 = 1;
    static OpCounter s_opCounter;
};

#endif /*  INCLUDED_OPTRACKER_H */
