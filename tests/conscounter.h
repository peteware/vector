#ifndef INCLUDED_PW_CONSCOUNTER_T_H
#define INCLUDED_PW_CONSCOUNTER_T_H

/**
 * Collect calls to constructors
 */
class ConsCounter
{
public:
    ConsCounter();

    ConsCounter& operator-=(ConsCounter const& op2);
    ConsCounter  operator-(ConsCounter const& op2) const;
    int          constructorCount() const;
    int          destructorCount() const;
    int          assignmentCount() const;
    bool         zero() const;
    ConsCounter& addDefault();
    int          getDefault() const;
    ConsCounter& addCopy();
    int          getCopy() const;
    ConsCounter& addMove();
    int          getMove() const;
    ConsCounter& addDefaultAlloc();
    int          getDefaultAlloc() const;
    ConsCounter& addCopyAlloc();
    int          getCopyAlloc() const;
    ConsCounter& addDestructor();
    int          getDestructor() const;
    ConsCounter& addAssignment();
    int          getAssignment() const;
    ConsCounter& addMoveAssignment();
    int          getMoveAssignment() const;

private:
    int m_default; ///< Default constructor
    int m_copy; ///< Copy constructor
    int m_move; ///< Move constructor
    int m_defaultalloc; ///< Default constructor with allocator
    int m_copyalloc; ///< Copy constructor with allocator
    int m_destructor; ///< destructors
    int m_assignment; ///< Calls to operator=()
    int m_moveassignment; ///< Calls to operator=()
};

#endif /*  INCLUDED_PW_CONSCOUNTER_T_H */
