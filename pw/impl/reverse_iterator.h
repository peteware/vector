#ifndef INCLUDED_PW_IMPL_REVERSE_ITERATOR_H
#define INCLUDED_PW_IMPL_REVERSE_ITERATOR_H

#include <pw/impl/advance.h>
#include <pw/impl/iterator_traits.h>

namespace pw {

template<class Iterator>
class reverse_iterator
{
public:
    using iterator_type     = Iterator;
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using value_type        = typename std::iterator_traits<Iterator>::value_type;
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using pointer           = typename std::iterator_traits<Iterator>::pointer;
    using reference         = typename std::iterator_traits<Iterator>::reference;

    constexpr reverse_iterator();
    constexpr explicit reverse_iterator(iterator_type iterator);
    template<class Other>
    constexpr reverse_iterator(const reverse_iterator<Other>& other);

    template<class Other>
    constexpr reverse_iterator& operator=(const reverse_iterator<Other>& other);

    constexpr reverse_iterator& operator++();
    constexpr reverse_iterator& operator++(int);
    constexpr reverse_iterator& operator--();
    constexpr reverse_iterator& operator--(int);
    constexpr reverse_iterator  operator+(difference_type n) const;
    constexpr reverse_iterator  operator-(difference_type n) const;
    constexpr reverse_iterator& operator+=(difference_type n);
    constexpr reverse_iterator& operator-=(difference_type n);

protected:
    iterator_type m_base;
}

template<class Iterator>
constexpr reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n,
                                               const reverse_iterator<Iterator>&                    iterator);
template<class Iterator>
constexpr reverse_iterator<Iterator> operator-(typename reverse_iterator<Iterator>::difference_type n,
                                               const reverse_iterator<Iterator>&                    iterator);
template<class Iterator1, class Iterator2>
constexpr bool operator==(const std::reverse_iterator<Iterator1>& op1,
                          const std::reverse_iterator<Iterator2>& op2);
template<class Iterator1, class Iterator2>
constexpr bool operator!=(const std::reverse_iterator<Iterator1>& op1,
                          const std::reverse_iterator<Iterator2>& op2);
template<class Iterator1, class Iterator2>
constexpr bool operator<(const std::reverse_iterator<Iterator1>& op1,
                         const std::reverse_iterator<Iterator2>& op2);
template<class Iterator1, class Iterator2>
constexpr bool operator<=(const std::reverse_iterator<Iterator1>& op1,
                          const std::reverse_iterator<Iterator2>& op2);
template<class Iterator1, class Iterator2>
constexpr bool operator>(const std::reverse_iterator<Iterator1>& op1,
                         const std::reverse_iterator<Iterator2>& op2);
template<class Iterator1, class Iterator2>
constexpr bool operator>=(const std::reverse_iterator<Iterator1>& op1,
                          const std::reverse_iterator<Iterator2>& op2);

template<class Iterator>
constexpr reverse_iterator<Iterator>::reverse_iterator()
    : m_base()
{
}

constexpr explicit template<class Iterator>
constexpr reverse_iterator<Iterator>::reverse_iterator(iterator_type iterator)
    : m_base(--iterator)
{
}

template<class Iterator>
template<class Other>
constexpr reverse_iterator<Iterator>::reverse_iterator(const reverse_iterator<Other>& other)
    : m_base(other.m_base)
{
}

template<class Iterator>
template<class Other>
constexpr reverse_iterator<Iterator>::reverse_iterator&
reverse_iterator<Iterator>::operator=(const reverse_iterator<Other>& other)
{
    m_base = other.m_base;
}

/**
 * This does not return m_base but rather what
 * the original iterator would've pointed to.
 *
 * @code
 * pw::reverse_iterator(iterator).base() == iterator;
 * @endcode
 */
template<class Iterator>
constexpr reverse_iterator<Iterator>::iterator_type
reverse_iterator<Iterator>::base() const
{
    iterator_type iterator = m_base;
    return ++iterator;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator++()
{
    --m_base;
    return *this;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator++(int)
{
    reverse_iterator iterator(*this);
    --m_base;
    return iterator;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator--()
{
    ++m_base;
    return *this;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator--(int)
{
    reverse_iterator iterator(*this);
    ++m_base;
    return iterator;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>
reverse_iterator<Iterator>::operator+(difference_type n)
{
    reverse_iterator iterator = this;
    advance(iterator, n);
    return iterator;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>
reverse_iterator<Iterator>::operator-(difference_type n)
{
    reverse_iterator iterator = this;
    advance(iterator, -n);
    return iterator;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator+=(difference_type n)
{
    advance(*this, n);
    return *this;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator-=(difference_type n)
{
    advance(*this, -n);
    return *this;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& iterator)
{
    return iterator + n;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>
operator-(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& iterator)
{
    return iterator - n;
}

template<class Iterator1, class Iterator2>
constexpr bool
operator==(const std::reverse_iterator<Iterator1>& op1, const std::reverse_iterator<Iterator2>& op2)
{
    return op1.m_base == op2.m_base;
}

template<class Iterator1, class Iterator2>
constexpr bool
operator!=(const std::reverse_iterator<Iterator1>& op1, const std::reverse_iterator<Iterator2>& op2)
{
    return op1.m_base != op2.m_base;
}

template<class Iterator1, class Iterator2>
constexpr bool
operator<(const std::reverse_iterator<Iterator1>& op1, const std::reverse_iterator<Iterator2>& op2)
{
    return op1.m_base > op2.m_base;
}

template<class Iterator1, class Iterator2>
constexpr bool
operator<=(const std::reverse_iterator<Iterator1>& op1, const std::reverse_iterator<Iterator2>& op2)
{
    return op1.m_base >= op2.m_base;
}

template<class Iterator1, class Iterator2>
constexpr bool
operator>(const std::reverse_iterator<Iterator1>& op1, const std::reverse_iterator<Iterator2>& op2)
{
    return op1.m_base < op2.m_base;
}

template<class Iterator1, class Iterator2>
constexpr bool
operator>=(const std::reverse_iterator<Iterator1>& op1, const std::reverse_iterator<Iterator2>& op2)
{
    return op1.m_base <= op2.m_base;
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_REVERSE_ITERATOR_H */
