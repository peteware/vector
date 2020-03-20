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
    int          allCount() const;
    bool         zero() const;
    ConsCounter& addDefaultConstructor();
    int          getDefaultConstructor() const;
    ConsCounter& addCopyConstructor();
    int          getCopyConstructor() const;
    ConsCounter& addMoveConstructor();
    int          getMoveConstructor() const;
    ConsCounter& addDefaultConstructorAlloc();
    int          getDefaultConstructorAlloc() const;
    ConsCounter& addCopyConstructorAlloc();
    int          getCopyConstructorAlloc() const;
    ConsCounter& addDestructor();
    int          getDestructor() const;
    ConsCounter& addAssignment();
    int          getAssignment() const;
    ConsCounter& addMoveAssignment();
    int          getMoveAssignment() const;
    ConsCounter& addPlusAssignment();
    int          getPlusAssignment() const;
    ConsCounter& addMinusAssignment();
    int          getMinusAssignment() const;
    ConsCounter& addEqual();
    int          getEqual() const;
    ConsCounter& addNotEqual();
    int          getNotEqual() const;
    ConsCounter& addLt();
    int          getLt() const;
    int          getComparisons();
    ConsCounter& addIncrement();
    int          getIncrement() const;
    ConsCounter& addDecrement();
    int          getDecrement() const;
    ConsCounter& addPlus();
    int          getPlus() const;
    ConsCounter& addMinus();
    int          getMinus() const;
    int          getAddition() const;
    int          getSubtraction() const;

private:
    int m_default         = 0; ///< Default constructor
    int m_copy            = 0; ///< Copy constructor
    int m_move            = 0; ///< Move constructor
    int m_defaultalloc    = 0; ///< Default constructor with allocator
    int m_copyalloc       = 0; ///< Copy constructor with allocator
    int m_destructor      = 0; ///< destructors
    int m_assignment      = 0; ///< Calls to operator=()
    int m_moveassignment  = 0; ///< Calls to operator=()
    int m_plusassignment  = 0; ///< Calls to operator+=()
    int m_minusassignment = 0; ///< Calls to operator-=()
    int m_equal           = 0; ///< Calls to operator==()
    int m_notEqual        = 0; ///< Calls to operator!=()
    int m_lt              = 0; ///< Calls to operator<()
    int m_increment       = 0; ///< Calls to operator++()
    int m_decrement       = 0; ///< Calls to operator--()
    int m_plus            = 0; ///< Calls to operator+()
    int m_minus           = 0; ///< Calls to operator-()
};

#endif /*  INCLUDED_PW_CONSCOUNTER_T_H */
