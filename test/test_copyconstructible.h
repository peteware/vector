#ifndef INCLUDED_TEST_COPYCONSTRUCTIBLE_H
#define INCLUDED_TEST_COPYCONSTRUCTIBLE_H

#include <test_optracker.h>

namespace pw { namespace test {
struct CopyConstructible : public OpTracker
{
    static OpCounter getCounter();

    CopyConstructible(int value)
        : OpTracker(s_opCounter, value)
    {
    }

    CopyConstructible(CopyConstructible const& copy)
        : OpTracker(copy)
    {
    }

    CopyConstructible(CopyConstructible&& move) noexcept
        : OpTracker(pw::move(move))
    {
    }

    ~CopyConstructible()
    {
    }

    CopyConstructible& operator=(CopyConstructible const& copy)
    {
        OpTracker::operator=(copy);
        return *this;
    }

    CopyConstructible& operator=(CopyConstructible&& move)
    {
        OpTracker::operator=(pw::move(move));
        return *this;
    }

    using OpTracker::operator==;
    // using OpTracker::operator!=;
    // using OpTracker::operator<;

private:
    static OpCounter s_opCounter;
};

bool permute(CopyConstructible& value, int depth);
}} // namespace pw::test

#endif /*  INCLUDED_TEST_COPYCONSTRUCTIBLE_H */
