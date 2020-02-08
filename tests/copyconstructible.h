#ifndef INCLUDED_COPYCONSTRUCTIBLE_H
#define INCLUDED_COPYCONSTRUCTIBLE_H

#include "conscounter.h"

struct CopyConstructible
{
    static ConsCounter getCounter();

    CopyConstructible();
    CopyConstructible(CopyConstructible const& copy);
    CopyConstructible(CopyConstructible&& copy);
    ~CopyConstructible();

    CopyConstructible& operator=(CopyConstructible const& copy);
    CopyConstructible& operator=(CopyConstructible&& copy);

private:
    static ConsCounter s_consCounter;
};

#endif /*  INCLUDED_COPYCONSTRUCTIBLE_H */
