#ifndef INCLUDED_PW_TEST_OPCOUNTER_H
#define INCLUDED_PW_TEST_OPCOUNTER_H

#include <iosfwd>

namespace pw::test {

/**
 * Collect calls to constructors
 */
class OpCounter
{
public:
    OpCounter() = default;

    OpCounter&         operator-=(OpCounter const& op2);
    OpCounter          operator-(OpCounter const& op2) const;
    [[nodiscard]] bool zero() const;
    [[nodiscard]] int  allCount() const;
    [[nodiscard]] int  constructorCount() const;
    [[nodiscard]] int  destructorCount() const;
    [[nodiscard]] int  assignmentCount() const;
    [[nodiscard]] int  additionCount() const;
    [[nodiscard]] int  subtractionCount() const;
    [[nodiscard]] int  arithmeticCount() const;
    [[nodiscard]] int  comparisonCount() const;
    OpCounter&         addDefaultConstructor();
    [[nodiscard]] int  getDefaultConstructor() const;
    OpCounter&         addCopyConstructor();
    [[nodiscard]] int  getCopyConstructor() const;
    OpCounter&         addMoveConstructor();
    [[nodiscard]] int  getMoveConstructor() const;
    OpCounter&         addDefaultConstructorAlloc();
    [[nodiscard]] int  getDefaultConstructorAlloc() const;
    OpCounter&         addCopyConstructorAlloc();
    [[nodiscard]] int  getCopyConstructorAlloc() const;
    OpCounter&         addOtherConstructor();
    [[nodiscard]] int  getOtherConstructor() const;
    OpCounter&         addDestructor();
    [[nodiscard]] int  getDestructor() const;
    OpCounter&         addAssignment();
    [[nodiscard]] int  getAssignment() const;
    OpCounter&         addMoveAssignment();
    [[nodiscard]] int  getMoveAssignment() const;
    OpCounter&         addPlusAssignment();
    [[nodiscard]] int  getPlusAssignment() const;
    OpCounter&         addMinusAssignment();
    [[nodiscard]] int  getMinusAssignment() const;
    OpCounter&         addEqual();
    [[nodiscard]] int  getEqual() const;
    OpCounter&         addNotEqual();
    [[nodiscard]] int  getNotEqual() const;
    OpCounter&         addLt();
    [[nodiscard]] int  getLt() const;
    OpCounter&         addIncrement();
    [[nodiscard]] int  getIncrement() const;
    OpCounter&         addDecrement();
    [[nodiscard]] int  getDecrement() const;
    OpCounter&         addPlus();
    [[nodiscard]] int  getPlus() const;
    OpCounter&         addMinus();
    [[nodiscard]] int  getMinus() const;

    OpCounter&         addAllocatorFirst();
    [[nodiscard]] int  getAllocatorFirst() const;
    OpCounter&         addAllocatorLast();
    [[nodiscard]] int  getAllocatorLast() const;
    OpCounter&         addAllocatorOnly();
    [[nodiscard]] int  getAllocatorOnly() const;
    OpCounter&         addNoAllocator();
    [[nodiscard]] int  getNoAllocator() const;

    std::ostream&      print(std::ostream& out) const;

private:
    int m_default          = 0; ///< Default constructor
    int m_copy             = 0; ///< Copy constructor
    int m_move             = 0; ///< Move constructor
    int m_default_alloc    = 0; ///< Default constructor with allocator
    int m_copy_alloc       = 0; ///< Copy constructor with allocator
    int m_other            = 0; ///< Any other constructor
    int m_destructor       = 0; ///< destructors
    int m_assignment       = 0; ///< Calls to operator=()
    int m_move_assignment  = 0; ///< Calls to operator=()
    int m_plus_assignment  = 0; ///< Calls to operator+=()
    int m_minus_assignment = 0; ///< Calls to operator-=()
    int m_increment        = 0; ///< Calls to operator++()
    int m_decrement        = 0; ///< Calls to operator--()
    int m_plus             = 0; ///< Calls to operator+()
    int m_minus            = 0; ///< Calls to operator-()
    int m_equal            = 0; ///< Calls to operator==()
    int m_notEqual         = 0; ///< Calls to operator!=()
    int m_lt               = 0; ///< Calls to operator<()
    int m_allocatorFirst   = 0; ///< Allocator passed as first argument
    int m_allocatorLast    = 0; ///< Allocator passed as last argument
    int m_allocatorOnly    = 0; ///< Only allocator passed
    int m_noAllocator      = 0; ///< No allocator passed
};

std::ostream& operator<<(std::ostream& out, OpCounter const& opCounter);
} // namespace pw::test

#endif /*  INCLUDED_PW_TEST_OPCOUNTER_H */
