#include "conscounter.h"

ConsCounter::ConsCounter()
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
ConsCounter::destructorCount() const
{
    return m_destructor;
}

int
ConsCounter::assignmentCount() const
{
    return m_assignment + m_moveassignment;
}
int
ConsCounter::allCount() const
{
    return m_default + m_copy + m_move + m_defaultalloc + m_copyalloc + m_destructor + m_assignment +
        m_moveassignment;
}

int
ConsCounter::getDefaultConstructor() const
{
    return m_default;
}

ConsCounter&
ConsCounter::addDefaultConstructor()
{
    ++m_default;
    return *this;
}

int
ConsCounter::getCopyConstructor() const
{
    return m_copy;
}

ConsCounter&
ConsCounter::addCopyConstructor()
{
    ++m_copy;
    return *this;
}

int
ConsCounter::getMoveConstructor() const
{
    return m_move;
}

ConsCounter&
ConsCounter::addMoveConstructor()
{
    ++m_move;
    return *this;
}

int
ConsCounter::getDefaultConstructorAlloc() const
{
    return m_defaultalloc;
}

ConsCounter&
ConsCounter::addDefaultConstructorAlloc()
{
    ++m_defaultalloc;
    return *this;
}

int
ConsCounter::getCopyConstructorAlloc() const
{
    return m_copyalloc;
}

ConsCounter&
ConsCounter::addCopyConstructorAlloc()
{
    ++m_copyalloc;
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

ConsCounter&
ConsCounter::addEqual()
{
    ++m_equal;
    return *this;
}

int
ConsCounter::getEqual() const
{
    return m_equal;
}

ConsCounter&
ConsCounter::addNotEqual()
{
    ++m_notEqual;
    return *this;
}

int
ConsCounter::getNotEqual() const
{
    return m_notEqual;
}

ConsCounter&
ConsCounter::addLt()
{
    ++m_lt;
    return *this;
}

int
ConsCounter::getLt() const
{
    return m_lt;
}

int
ConsCounter::getComparisons()
{
    return m_equal + m_notEqual + m_lt;
}

ConsCounter&
ConsCounter::operator-=(ConsCounter const& op2)
{
    m_default -= op2.m_default;
    m_copy -= op2.m_copy;
    m_move -= op2.m_move;
    m_defaultalloc -= op2.m_defaultalloc;
    m_copyalloc -= op2.m_copyalloc;
    m_destructor -= op2.m_destructor;
    m_assignment -= op2.m_assignment;
    m_moveassignment -= op2.m_moveassignment;
    m_equal -= op2.m_equal;
    m_notEqual -= op2.m_notEqual;
    m_lt -= op2.m_lt;

    return *this;
}

ConsCounter
ConsCounter::operator-(ConsCounter const& op2) const
{
    ConsCounter c(*this);

    c -= op2;
    return c;
}
