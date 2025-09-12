#include <test_opcounter.h>

#include <iostream>

namespace pw::test {

/**
 * @return true if no constructors called (constructorCount() == 0)
 */
bool
OpCounter::zero() const
{
    return constructorCount() == 0;
}

/**
 * @return constructorCount() + destructor + assignment count
 */
int
OpCounter::allCount() const
{
    return constructorCount() + destructorCount() + assignmentCount();
}

/**
 * @brief
 * @return default + copy + move + default_alloc + copy_alloc + other constructor counts
 */
int
OpCounter::constructorCount() const
{
    return m_default_constructor + m_copy_constructor + m_move_constructor + m_default_constructor_alloc +
        m_copy_constructor_alloc + m_other_constructor;
}

/**
 * @return destructor calls
 */
int
OpCounter::destructorCount() const
{
    return m_destructor;
}

int
OpCounter::assignmentCount() const
{
    return m_assignment + m_move_assignment;
}

int
OpCounter::additionCount() const
{
    return m_plus_assignment + m_plus + m_increment;
}

int
OpCounter::subtractionCount() const
{
    return m_minus_assignment + m_minus + m_decrement;
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
    return m_default_constructor;
}

OpCounter&
OpCounter::addDefaultConstructor()
{
    ++m_default_constructor;
    return *this;
}

int
OpCounter::getCopyConstructor() const
{
    return m_copy_constructor;
}

OpCounter&
OpCounter::addCopyConstructor()
{
    ++m_copy_constructor;
    return *this;
}

int
OpCounter::getMoveConstructor() const
{
    return m_move_constructor;
}

OpCounter&
OpCounter::addMoveConstructor()
{
    ++m_move_constructor;
    return *this;
}

int
OpCounter::getDefaultConstructorAlloc() const
{
    return m_default_constructor_alloc;
}

OpCounter&
OpCounter::addDefaultConstructorAlloc()
{
    ++m_default_constructor_alloc;
    return *this;
}

int
OpCounter::getCopyConstructorAlloc() const
{
    return m_copy_constructor_alloc;
}

OpCounter&
OpCounter::addCopyConstructorAlloc()
{
    ++m_copy_constructor_alloc;
    return *this;
}

int
OpCounter::getOtherConstructor() const
{
    return m_other_constructor;
}

OpCounter&
OpCounter::addOtherConstructor()
{
    ++m_other_constructor;
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
    ++m_move_assignment;
    return *this;
}

int
OpCounter::getMoveAssignment() const
{
    return m_move_assignment;
}

OpCounter&
OpCounter::addPlusAssignment()
{
    ++m_plus_assignment;
    return *this;
}

int
OpCounter::getPlusAssignment() const
{
    return m_plus_assignment;
}

OpCounter&
OpCounter::addMinusAssignment()
{
    ++m_minus_assignment;
    return *this;
}

int
OpCounter::getMinusAssignment() const
{
    return m_minus_assignment;
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
OpCounter::addAllocatorFirst()
{
    ++m_allocatorFirst;
    return *this;
}

int
OpCounter::getAllocatorFirst() const
{
    return m_allocatorFirst;
}

OpCounter&
OpCounter::addAllocatorLast()
{
    ++m_allocatorLast;
    return *this;
}

int
OpCounter::getAllocatorLast() const
{
    return m_allocatorLast;
}

OpCounter&
OpCounter::addAllocatorOnly()
{
    ++m_allocatorOnly;
    return *this;
}

int
OpCounter::getAllocatorOnly() const
{
    return m_allocatorOnly;
}

OpCounter&
OpCounter::addNoAllocator()
{
    ++m_noAllocator;
    return *this;
}

int
OpCounter::getNoAllocator() const
{
    return m_noAllocator;
}

std::ostream&
OpCounter::print(std::ostream& out) const
{
    if (m_default_constructor)
        out << " default = " << m_default_constructor;
    if (m_copy_constructor)
        out << " copy = " << m_copy_constructor;
    if (m_move_constructor)
        out << " move = " << m_move_constructor;
    if (m_default_constructor_alloc)
        out << " defaultalloc = " << m_default_constructor_alloc;
    if (m_copy_constructor_alloc)
        out << " copyalloc = " << m_copy_constructor_alloc;
    if (m_other_constructor)
        out << " other = " << m_other_constructor;
    if (m_destructor)
        out << " destructor = " << m_destructor;
    if (m_assignment)
        out << " assignment = " << m_assignment;
    if (m_move_assignment)
        out << " moveassignment = " << m_move_assignment;
    if (m_plus_assignment)
        out << " plusassignment = " << m_plus_assignment;
    if (m_minus_assignment)
        out << " minusassignment = " << m_minus_assignment;
    if (m_increment)
        out << " increment = " << m_increment;
    if (m_decrement)
        out << " decrement = " << m_decrement;
    if (m_plus)
        out << " plus = " << m_plus;
    if (m_minus)
        out << " minus = " << m_minus;
    if (m_equal)
        out << " equal = " << m_equal;
    if (m_notEqual)
        out << " notEqual = " << m_notEqual;
    if (m_lt)
        out << " lt = " << m_lt;
    if (m_allocatorFirst)
        out << " allocatorFirst = " << m_allocatorFirst;
    if (m_allocatorLast)
        out << " allocatorLast = " << m_allocatorLast;
    if (m_allocatorOnly)
        out << " allocatorOnly = " << m_allocatorOnly;
    if (m_noAllocator)
        out << " noAllocator = " << m_noAllocator;

    return out;
}

OpCounter&
OpCounter::operator-=(OpCounter const& op2)
{
    m_default_constructor -= op2.m_default_constructor;
    m_copy_constructor -= op2.m_copy_constructor;
    m_move_constructor -= op2.m_move_constructor;
    m_default_constructor_alloc -= op2.m_default_constructor_alloc;
    m_copy_constructor_alloc -= op2.m_copy_constructor_alloc;
    m_other_constructor -= op2.m_other_constructor;
    m_destructor -= op2.m_destructor;
    m_assignment -= op2.m_assignment;
    m_move_assignment -= op2.m_move_assignment;
    m_plus_assignment -= op2.m_plus_assignment;
    m_minus_assignment -= op2.m_minus_assignment;
    m_increment -= op2.m_increment;
    m_decrement -= op2.m_decrement;
    m_plus -= op2.m_plus;
    m_minus -= op2.m_minus;
    m_equal -= op2.m_equal;
    m_notEqual -= op2.m_notEqual;
    m_lt -= op2.m_lt;
    m_allocatorFirst -= op2.m_allocatorFirst;
    m_allocatorLast -= op2.m_allocatorLast;
    m_allocatorOnly -= op2.m_allocatorOnly;
    m_noAllocator -= op2.m_noAllocator;

    return *this;
}

OpCounter
OpCounter::operator-(OpCounter const& op2) const
{
    OpCounter c(*this);

    c -= op2;
    return c;
}

std::ostream&
operator<<(std::ostream& out, OpCounter const& opCounter)
{
    return opCounter.print(out);
}
} // namespace pw::test
