#ifndef INCLUDED_PW_TEST_OPTRACKER_DEFAULTCOPYCONSTRUCTIBLE_H
#define INCLUDED_PW_TEST_OPTRACKER_DEFAULTCOPYCONSTRUCTIBLE_H

#include <pw/impl/utility/move.h>

#include <compare>
#include <test_optracker.h>

namespace pw::test {
struct OpTrackerDefaultCopyConstructible : public OpTracker
{
    static OpCounter& getCounter() { return s_opCounter; }

    OpTrackerDefaultCopyConstructible()
        : OpTracker(s_opCounter)
    {
        getCounter().addDefaultConstructor();
    }

    OpTrackerDefaultCopyConstructible(int value) // NOLINT(*-explicit-constructor)
        : OpTracker(s_opCounter, value)
    {
        getCounter().addOtherConstructor();
    }

    OpTrackerDefaultCopyConstructible(OpTrackerDefaultCopyConstructible const& copy)
        : OpTracker(copy)
    {
        //getCounter().addCopyConstructor();
    }

    OpTrackerDefaultCopyConstructible(OpTrackerDefaultCopyConstructible&& move) noexcept
        : OpTracker(pw::move(move))
    {
        //getCounter().addMoveConstructor();
    }

    OpTrackerDefaultCopyConstructible& operator=(OpTrackerDefaultCopyConstructible const& copy)
    {
        OpTracker::operator=(copy);
        return *this;
    }

    OpTrackerDefaultCopyConstructible& operator=(OpTrackerDefaultCopyConstructible&& move) noexcept
    {
        OpTracker::operator=(pw::move(move));
        return *this;
    }

    bool operator==(OpTrackerDefaultCopyConstructible const& op2) const { return OpTracker::operator==(op2); }
    bool operator!=(OpTrackerDefaultCopyConstructible const& op2) const { return OpTracker::operator!=(op2); }
    std::strong_ordering operator<=>(OpTrackerDefaultCopyConstructible const& op2) const;
    // using OpTracker::operator<;

private:
    static OpCounter s_opCounter;
};

bool permute(OpTrackerDefaultCopyConstructible& value, int depth);
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_OPTRACKER_DEFAULTCOPYCONSTRUCTIBLE_H */
