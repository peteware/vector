#ifndef INCLUDED_PW_OPCOUNTER_T_H
#define INCLUDED_PW_OPCOUNTER_T_H

namespace pw { namespace test {

/**
 * Collect calls to constructors
 */
class OpCounter
{
public:
    OpCounter();

    OpCounter& operator-=(OpCounter const& op2);
    OpCounter  operator-(OpCounter const& op2) const;
    bool       zero() const;
    int        allCount() const;
    int        constructorCount() const;
    int        destructorCount() const;
    int        assignmentCount() const;
    int        additionCount() const;
    int        subtractionCount() const;
    int        arithmeticCount() const;
    int        comparisonCount() const;
    OpCounter& addDefaultConstructor();
    int        getDefaultConstructor() const;
    OpCounter& addCopyConstructor();
    int        getCopyConstructor() const;
    OpCounter& addMoveConstructor();
    int        getMoveConstructor() const;
    OpCounter& addDefaultConstructorAlloc();
    int        getDefaultConstructorAlloc() const;
    OpCounter& addCopyConstructorAlloc();
    int        getCopyConstructorAlloc() const;
    OpCounter& addOtherConstructor();
    int        getOtherConstructor() const;
    OpCounter& addDestructor();
    int        getDestructor() const;
    OpCounter& addAssignment();
    int        getAssignment() const;
    OpCounter& addMoveAssignment();
    int        getMoveAssignment() const;
    OpCounter& addPlusAssignment();
    int        getPlusAssignment() const;
    OpCounter& addMinusAssignment();
    int        getMinusAssignment() const;
    OpCounter& addEqual();
    int        getEqual() const;
    OpCounter& addNotEqual();
    int        getNotEqual() const;
    OpCounter& addLt();
    int        getLt() const;
    OpCounter& addIncrement();
    int        getIncrement() const;
    OpCounter& addDecrement();
    int        getDecrement() const;
    OpCounter& addPlus();
    int        getPlus() const;
    OpCounter& addMinus();
    int        getMinus() const;

private:
    int m_default         = 0; ///< Default constructor
    int m_copy            = 0; ///< Copy constructor
    int m_move            = 0; ///< Move constructor
    int m_defaultalloc    = 0; ///< Default constructor with allocator
    int m_copyalloc       = 0; ///< Copy constructor with allocator
    int m_other           = 0; ///< Any other constructor
    int m_destructor      = 0; ///< destructors
    int m_assignment      = 0; ///< Calls to operator=()
    int m_moveassignment  = 0; ///< Calls to operator=()
    int m_plusassignment  = 0; ///< Calls to operator+=()
    int m_minusassignment = 0; ///< Calls to operator-=()
    int m_increment       = 0; ///< Calls to operator++()
    int m_decrement       = 0; ///< Calls to operator--()
    int m_plus            = 0; ///< Calls to operator+()
    int m_minus           = 0; ///< Calls to operator-()
    int m_equal           = 0; ///< Calls to operator==()
    int m_notEqual        = 0; ///< Calls to operator!=()
    int m_lt              = 0; ///< Calls to operator<()
};

}} // namespace pw::test

#endif /*  INCLUDED_PW_OPCOUNTER_T_H */
