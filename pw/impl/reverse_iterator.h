#ifndef INCLUDED_PW_IMPL_REVERSE_ITERATOR_H
#define INCLUDED_PW_IMPL_REVERSE_ITERATOR_H

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

protected:
    iterator_type m_base;
}

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
    : m_base(other.base())
{
}

template<class Iterator>
template<class Other>
constexpr reverse_iterator<Iterator>::reverse_iterator&
reverse_iterator<Iterator>::operator=(const reverse_iterator<Other>& other)
{
    m_base = other.base();
}

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
    return --m_base;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator++(int)
{
    return m_base--;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator--()
{
    return ++m_base;
}

template<class Iterator>
constexpr reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator--(int)
{
    return m_base++;
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_REVERSE_ITERATOR_H */
