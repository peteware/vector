#include "opcounter.h"

OpCounter::OpCounter()
{
}

bool
OpCounter::zero() const
{
    return constructorCount() == 0;
}

int
OpCounter::allCount() const
{
    return constructorCount() + destructorCount() + assignmentCount();
}

int
OpCounter::constructorCount() const
{
    return m_default + m_copy + m_move + m_defaultalloc + m_copyalloc;
}

int
OpCounter::destructorCount() const
{
    return m_destructor;
}

int
OpCounter::assignmentCount() const
{
    return m_assignment + m_moveassignment;
}

int
OpCounter::additionCount() const
{
    return m_plusassignment + m_plus + m_increment;
}

int
OpCounter::subtractionCount() const
{
    return m_minusassignment + m_minus + m_decrement;
}

int
OpCounter::arithmeticCount() const
{
    return subtractionCount() + additionCount();
}

int
OpCounter::comparisonCount() const
{
    return m_equal + m_notEqual + m_lt;
}

int
OpCounter::getDefaultConstructor() const
{
    return m_default;
}

OpCounter&
OpCounter::addDefaultConstructor()
{
    ++m_default;
    return *this;
}

int
OpCounter::getCopyConstructor() const
{
    return m_copy;
}

OpCounter&
OpCounter::addCopyConstructor()
{
    ++m_copy;
    return *this;
}

int
OpCounter::getMoveConstructor() const
{
    return m_move;
}

OpCounter&
OpCounter::addMoveConstructor()
{
    ++m_move;
    return *this;
}

int
OpCounter::getDefaultConstructorAlloc() const
{
    return m_defaultalloc;
}

OpCounter&
OpCounter::addDefaultConstructorAlloc()
{
    ++m_defaultalloc;
    return *this;
}

int
OpCounter::getCopyConstructorAlloc() const
{
    return m_copyalloc;
}

OpCounter&
OpCounter::addCopyConstructorAlloc()
{
    ++m_copyalloc;
    return *this;
}

OpCounter&
OpCounter::addDestructor()
{
    ++m_destructor;
    return *this;
}

int
OpCounter::getDestructor() const
{
    return m_destructor;
}

OpCounter&
OpCounter::addAssignment()
{
    ++m_assignment;
    return *this;
}

int
OpCounter::getAssignment() const
{
    return m_assignment;
}

OpCounter&
OpCounter::addMoveAssignment()
{
    ++m_moveassignment;
    return *this;
}

int
OpCounter::getMoveAssignment() const
{
    return m_moveassignment;
}

OpCounter&
OpCounter::addPlusAssignment()
{
    ++m_plusassignment;
    return *this;
}

int
OpCounter::getPlusAssignment() const
{
    return m_plusassignment;
}

OpCounter&
OpCounter::addMinusAssignment()
{
    ++m_minusassignment;
    return *this;
}

int
OpCounter::getMinusAssignment() const
{
    return m_minusassignment;
}

OpCounter&
OpCounter::addIncrement()
{
    ++m_increment;
    return *this;
}

int
OpCounter::getIncrement() const
{
    return m_increment;
}

OpCounter&
OpCounter::addDecrement()
{
    ++m_decrement;
    return *this;
}

int
OpCounter::getDecrement() const
{
    return m_decrement;
}

OpCounter&
OpCounter::addPlus()
{
    ++m_plus;
    return *this;
}

int
OpCounter::getPlus() const
{
    return m_plus;
}

OpCounter&
OpCounter::addMinus()
{
    ++m_minus;
    return *this;
}

int
OpCounter::getMinus() const
{
    return m_minus;
}

OpCounter&
OpCounter::addEqual()
{
    ++m_equal;
    return *this;
}

int
OpCounter::getEqual() const
{
    return m_equal;
}

OpCounter&
OpCounter::addNotEqual()
{
    ++m_notEqual;
    return *this;
}

int
OpCounter::getNotEqual() const
{
    return m_notEqual;
}

OpCounter&
OpCounter::addLt()
{
    ++m_lt;
    return *this;
}

int
OpCounter::getLt() const
{
    return m_lt;
}

OpCounter&
OpCounter::operator-=(OpCounter const& op2)
{
    m_default -= op2.m_default;
    m_copy -= op2.m_copy;
    m_move -= op2.m_move;
    m_defaultalloc -= op2.m_defaultalloc;
    m_copyalloc -= op2.m_copyalloc;
    m_destructor -= op2.m_destructor;
    m_assignment -= op2.m_assignment;
    m_moveassignment -= op2.m_moveassignment;
    m_plusassignment -= op2.m_plusassignment;
    m_minusassignment -= op2.m_minusassignment;
    m_increment -= op2.m_increment;
    m_decrement -= op2.m_decrement;
    m_plus -= op2.m_plus;
    m_minus -= op2.m_minus;
    m_equal -= op2.m_equal;
    m_notEqual -= op2.m_notEqual;
    m_lt -= op2.m_lt;

    return *this;
}

OpCounter
OpCounter::operator-(OpCounter const& op2) const
{
    OpCounter c(*this);

    c -= op2;
    return c;
}
