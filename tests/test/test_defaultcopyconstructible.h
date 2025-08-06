#ifndef INCLUDED_PW_TEST_DEFAULTCOPYCONSTRUCTIBLE_H
#define INCLUDED_PW_TEST_DEFAULTCOPYCONSTRUCTIBLE_H

#include <compare>
#include <test_optracker.h>

namespace pw::test {
struct DefaultCopyConstructible : public OpTracker
{
    static OpCounter getCounter();

    DefaultCopyConstructible()
        : OpTracker(s_opCounter)
    {
    }

    DefaultCopyConstructible(int value)
        : OpTracker(s_opCounter, value)
    {
    }

    DefaultCopyConstructible(DefaultCopyConstructible const& copy)
        : OpTracker(copy)
    {
    }

    DefaultCopyConstructible(DefaultCopyConstructible&& move) noexcept
        : OpTracker(pw::move(move))
    {
    }

    ~DefaultCopyConstructible() { }

    DefaultCopyConstructible& operator=(DefaultCopyConstructible const& copy)
    {
        OpTracker::operator=(copy);
        return *this;
    }

    DefaultCopyConstructible& operator=(DefaultCopyConstructible&& move)
    {
        OpTracker::operator=(pw::move(move));
        return *this;
    }

    bool operator==(DefaultCopyConstructible const& op2) const { return OpTracker::operator==(op2); }
    bool operator!=(DefaultCopyConstructible const& op2) const { return OpTracker::operator!=(op2); }
    std::strong_ordering operator<=>(DefaultCopyConstructible const& op2) const;
    // using OpTracker::operator<;

private:
    static OpCounter s_opCounter;
};

bool permute(DefaultCopyConstructible& value, int depth);
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_DEFAULTCOPYCONSTRUCTIBLE_H */
