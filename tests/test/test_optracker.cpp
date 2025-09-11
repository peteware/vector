#include <test_optracker.h>
#include <test_permute.h>

#include <iostream>

namespace pw::test {

OpTracker::OpTracker(OpCounter& opCounter)
    : m_opCounter(opCounter)
    , m_value(-1)
{
    // m_opCounter.addDefaultConstructor();
}

OpTracker::OpTracker(OpCounter& opCounter, value_type const& value)
    : m_opCounter(opCounter)
    , m_value(value)
{
    // m_opCounter.addOtherConstructor();
}

OpTracker::OpTracker(OpTracker const& copy)
    : m_opCounter(copy.m_opCounter)
    , m_value(copy.m_value)
{
    m_opCounter.addCopyConstructor();
}

OpTracker::OpTracker(OpTracker&& move) noexcept
    : m_opCounter(move.m_opCounter)
    , m_value(move.m_value)
{
    //copy.m_value *= -2;
    m_opCounter.addMoveConstructor();
}

OpTracker::~OpTracker()
{
    m_opCounter.addDestructor();
}

OpTracker::value_type
OpTracker::value() const
{
    return m_value;
}

OpTracker&
OpTracker::setValue(value_type const& value)
{
    m_value = value;
    return *this;
}

OpTracker&
OpTracker::reset()
{
    m_opCounter = OpCounter {};
    return *this;
}

OpCounter
OpTracker::opCounter() const
{
    return m_opCounter;
}

OpTracker&
OpTracker::operator=(OpTracker const& copy)
{
    m_value = copy.m_value;
    m_opCounter.addAssignment();
    return *this;
}

OpTracker&
OpTracker::operator=(OpTracker&& move)
{
    m_value      = move.m_value;
    move.m_value = -1;
    m_opCounter.addMoveAssignment();
    return *this;
}

bool
OpTracker::operator==(OpTracker const& op2) const
{
    m_opCounter.addEqual();
    return m_value == op2.m_value;
}

bool
OpTracker::operator!=(OpTracker const& op2) const
{
    m_opCounter.addNotEqual();
    return m_value != op2.m_value;
}

bool
OpTracker::operator<(OpTracker const& op2) const
{
    m_opCounter.addLt();
    return m_value < op2.m_value;
}

std::strong_ordering
OpTracker::operator<=>(OpTracker const& op2) const
{
    m_opCounter.addLt(); // Track the comparison operation
    if (m_value < op2.m_value)
    {
        return std::strong_ordering::less;
    }
    else if (m_value > op2.m_value)
    {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
}

bool
permute(OpTracker& value, int depth)
{
    int val = value.value();
    if (permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}

std::ostream&
operator<<(std::ostream& out, OpTracker const& op2)
{
    out << op2.value();
    return out;
}

} // namespace pw::test
