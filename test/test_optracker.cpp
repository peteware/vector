#include <test_optracker.h>
#include <test_permute.h>


namespace pw { namespace test {

OpTracker::OpTracker(OpCounter& opCounter)
    : m_opCounter(opCounter)
    , m_value(-1)
{
    m_opCounter.addDefaultConstructor();
}

OpTracker::OpTracker(OpCounter& opCounter, int value)
    : m_opCounter(opCounter)
    , m_value(value)
{
    m_opCounter.addOtherConstructor();
}

OpTracker::OpTracker(OpTracker const& copy)
    : m_value(copy.m_value)
    , m_opCounter(copy.m_opCounter)
{
    m_opCounter.addCopyConstructor();
}

OpTracker::OpTracker(OpTracker&& copy) noexcept
    : m_value(copy.m_value)
    , m_opCounter(copy.m_opCounter)
{
    copy.m_value = -2;
    m_opCounter.addMoveConstructor();
}

OpTracker::~OpTracker()
{
    m_opCounter.addDestructor();
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
    m_opCounter.addAssignment();
    return *this;
}

OpTracker&
OpTracker::operator=(OpTracker&& copy)
{
    copy.m_value = -1;
    m_value      = copy.m_value;
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

}} // namespace pw::test
