#ifndef INCLUDED_COPYCONSTRUCTIBLE_H
#define INCLUDED_COPYCONSTRUCTIBLE_H

#include "conscounter.h"

struct CopyConstructible
{
    static ConsCounter getCounter();

    CopyConstructible();
    CopyConstructible(CopyConstructible const& copy);
    CopyConstructible(CopyConstructible&& copy) noexcept;
    ~CopyConstructible();

    CopyConstructible& operator=(CopyConstructible const& copy);
    CopyConstructible& operator=(CopyConstructible&& copy);

private:
    int                m_value;
    static ConsCounter s_consCounter;
};

#endif /*  INCLUDED_COPYCONSTRUCTIBLE_H */
