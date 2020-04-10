#ifndef INCLUDED_COPYCONSTRUCTIBLE_H
#define INCLUDED_COPYCONSTRUCTIBLE_H

#include "opcounter.h"

struct CopyConstructible
{
    static OpCounter getCounter();

    CopyConstructible(int value = 0);
    CopyConstructible(CopyConstructible const& copy);
    CopyConstructible(CopyConstructible&& copy) noexcept;
    ~CopyConstructible();

    int                value() const;
    CopyConstructible& setValue(int value);

    CopyConstructible& operator=(CopyConstructible const& copy);
    CopyConstructible& operator=(CopyConstructible&& copy);
    bool               operator==(CopyConstructible const& op2) const;
    bool               operator!=(CopyConstructible const& op2) const;
    bool               operator<(CopyConstructible const& op2) const;

private:
    int              m_value;
    static OpCounter s_opCounter;
};

bool permute(CopyConstructible& value, int depth);

#endif /*  INCLUDED_COPYCONSTRUCTIBLE_H */
