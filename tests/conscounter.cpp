#include "conscounter.h"

ConsCounter::ConsCounter()
    : m_default(0)
    , m_copy(0)
    , m_move(0)
    , m_destructor(0)
    , m_assignment(0)
    , m_moveassignment(0)
    , m_defaultalloc(0)
    , m_copyalloc(0)
{
}

bool
ConsCounter::zero() const
{
    return constructorCount() == 0;
}

int
ConsCounter::constructorCount() const
{
    return m_default + m_copy + m_move + m_defaultalloc + m_copyalloc;
}

int
ConsCounter::getDefault() const
{
    return m_default;
}

ConsCounter&
ConsCounter::addDefault()
{
    ++m_default;
    return *this;
}

int
ConsCounter::getCopy() const
{
    return m_copy;
}

ConsCounter&
ConsCounter::addCopy()
{
    ++m_copy;
    return *this;
}

int
ConsCounter::getMove() const
{
    return m_move;
}

ConsCounter&
ConsCounter::addMove()
{
    ++m_move;
    return *this;
}

ConsCounter&
ConsCounter::addDestructor()
{
    ++m_destructor;
    return *this;
}

int
ConsCounter::getDestructor() const
{
    return m_destructor;
}

ConsCounter&
ConsCounter::addAssignment()
{
    ++m_assignment;
    return *this;
}

int
ConsCounter::getAssignment() const
{
    return m_assignment;
}

ConsCounter&
ConsCounter::addMoveAssignment()
{
    ++m_moveassignment;
    return *this;
}

int
ConsCounter::getMoveAssignment() const
{
    return m_moveassignment;
}

int
ConsCounter::getDefaultAlloc() const
{
    return m_defaultalloc;
}

ConsCounter&
ConsCounter::addDefaultAlloc()
{
    ++m_defaultalloc;
    return *this;
}

int
ConsCounter::getCopyAlloc() const
{
    return m_copyalloc;
}

ConsCounter&
ConsCounter::addCopyAlloc()
{
    ++m_copyalloc;
    return *this;
}

ConsCounter&
ConsCounter::operator-=(ConsCounter const& op2)
{
    m_default -= op2.m_default;
    m_copy -= op2.m_copy;
    m_move -= op2.m_move;
    m_destructor -= op2.m_destructor;
    m_assignment -= op2.m_assignment;
    m_moveassignment -= op2.m_moveassignment;
    m_defaultalloc -= op2.m_defaultalloc;
    m_copyalloc -= op2.m_copyalloc;

    return *this;
}

ConsCounter
ConsCounter::operator-(ConsCounter const& op2) const
{
    ConsCounter c(*this);

    c -= op2;
    return c;
}
