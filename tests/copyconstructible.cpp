#include "copyconstructible.h"
#include "permute.h"

OpCounter CopyConstructible::s_opCounter;

OpCounter
CopyConstructible::getCounter()
{
    return s_opCounter;
}

CopyConstructible::CopyConstructible(int value)
    : m_value(value)
{
    s_opCounter.addDefaultConstructor();
}

CopyConstructible::CopyConstructible(CopyConstructible const& copy)
    : m_value(copy.m_value)
{
    s_opCounter.addCopyConstructor();
}

CopyConstructible::CopyConstructible(CopyConstructible&& copy) noexcept
    : m_value(copy.m_value)
{
    copy.m_value = -2;
    s_opCounter.addMoveConstructor();
}

CopyConstructible::~CopyConstructible()
{
    s_opCounter.addDestructor();
}

int
CopyConstructible::value() const
{
    return m_value;
}

CopyConstructible&
CopyConstructible::setValue(int value)
{
    m_value = value;
    return *this;
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible const& copy)
{
    m_value = copy.m_value;
    s_opCounter.addAssignment();
    return *this;
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible&& copy)
{
    copy.m_value = -1;
    m_value      = copy.m_value;
    s_opCounter.addMoveAssignment();
    return *this;
}

bool
CopyConstructible::operator==(CopyConstructible const& op2) const
{
    s_opCounter.addEqual();
    return m_value == op2.m_value;
}

bool
CopyConstructible::operator!=(CopyConstructible const& op2) const
{
    s_opCounter.addNotEqual();
    return m_value != op2.m_value;
}

bool
CopyConstructible::operator<(CopyConstructible const& op2) const
{
    s_opCounter.addLt();
    return m_value < op2.m_value;
}

bool
permute(CopyConstructible& value, int depth)
{
    int val = value.value();
    if (permute(val, depth))
    {
        value.setValue(val);
        return true;
    }
    return false;
}
