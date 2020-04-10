#include "copyconstructible.h"
#include "permute.h"

OpCounter OpTracker::s_opCounter;

OpCounter
OpTracker::getCounter()
{
    return s_opCounter;
}

OpTracker::OpTracker(int value)
    : m_value(value)
{
    s_opCounter.addDefaultConstructor();
}

OpTracker::OpTracker(OpTracker const& copy)
    : m_value(copy.m_value)
{
    s_opCounter.addCopyConstructor();
}

OpTracker::OpTracker(OpTracker&& copy) noexcept
    : m_value(copy.m_value)
{
    copy.m_value = -2;
    s_opCounter.addMoveConstructor();
}

OpTracker::~OpTracker()
{
    s_opCounter.addDestructor();
}

int
OpTracker::value() const
{
    return m_value;
}

OpTracker&
OpTracker::setValue(int value)
{
    m_value = value;
    return *this;
}

OpTracker&
OpTracker::operator=(OpTracker const& copy)
{
    m_value = copy.m_value;
    s_opCounter.addAssignment();
    return *this;
}

OpTracker&
OpTracker::operator=(OpTracker&& copy)
{
    copy.m_value = -1;
    m_value      = copy.m_value;
    s_opCounter.addMoveAssignment();
    return *this;
}

bool
OpTracker::operator==(OpTracker const& op2) const
{
    s_opCounter.addEqual();
    return m_value == op2.m_value;
}

bool
OpTracker::operator!=(OpTracker const& op2) const
{
    s_opCounter.addNotEqual();
    return m_value != op2.m_value;
}

bool
OpTracker::operator<(OpTracker const& op2) const
{
    s_opCounter.addLt();
    return m_value < op2.m_value;
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
