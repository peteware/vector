#include "copyconstructible.h"

ConsCounter CopyConstructible::s_consCounter;

ConsCounter
CopyConstructible::getCounter()
{
    return s_consCounter;
}

CopyConstructible::CopyConstructible()
{
    s_consCounter.addDefault();
}

CopyConstructible::CopyConstructible(CopyConstructible const& copy)
{
    s_consCounter.addCopy();
}

CopyConstructible::CopyConstructible(CopyConstructible&& copy)
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
    s_consCounter.addAssignment();
    return *this;
}

CopyConstructible&
CopyConstructible::operator=(CopyConstructible&& copy)
{
    s_consCounter.addMoveAssignment();
    return *this;
}
