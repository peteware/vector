#include "copyconstructible.h"

ConsCounter CopyConstructible::s_consCounter;

ConsCounter
CopyConstructible::getCounter()
{
    return s_consCounter;
}

CopyConstructible::CopyConstructible()
    : m_value(0)
{
    s_consCounter.addDefault();
}

CopyConstructible::CopyConstructible(CopyConstructible const& copy)
    : m_value(copy.m_value)
{
    s_consCounter.addCopy();
}

CopyConstructible::CopyConstructible(CopyConstructible&& copy) noexcept
    : m_value(copy.m_value)
{
    s_consCounter.addMove();
}

CopyConstructible::~CopyConstructible()
{
    s_consCounter.addDestructor();
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible const& copy)
{
    m_value = copy.m_value;
    s_consCounter.addAssignment();
    return *this;
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible&& copy)
{
    m_value = copy.m_value;
    s_consCounter.addMoveAssignment();
    return *this;
}

bool
CopyConstructible::operator==(CopyConstructible const& op2) const
{
    s_consCounter.addEqual();
    return m_value == op2.m_value;
}

bool
CopyConstructible::operator!=(CopyConstructible const& op2) const
{
    s_consCounter.addNotEqual();
    return m_value != op2.m_value;
}

bool
CopyConstructible::operator<(CopyConstructible const& op2) const
{
    s_consCounter.addLt();
    return m_value < op2.m_value;
}
