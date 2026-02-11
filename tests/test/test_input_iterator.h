#ifndef INCLUDED_PW_TEST_INPUT_ITERATOR_H
#define INCLUDED_PW_TEST_INPUT_ITERATOR_H

#include <pw/impl/iterator/iterator_tag.h>
#include <pw/impl/iterator_traits.h>
#include <pw/impl/utility/move.h>

#include <memory> // Needed for std::shared_ptr

namespace pw::test {

template<class Iterator>
struct test_input_iterator
{
    using iterator_category = input_iterator_tag;
    using value_type        = iterator_traits<Iterator>::value_type;
    using difference_type   = iterator_traits<Iterator>::difference_type;
    using pointer           = iterator_traits<Iterator>::pointer;
    using reference         = iterator_traits<Iterator>::reference;

    test_input_iterator()   = default;
    explicit test_input_iterator(Iterator iterator);
    test_input_iterator(test_input_iterator const& copy);
    test_input_iterator(test_input_iterator&& other) noexcept;
    ~test_input_iterator() = default;

    test_input_iterator& operator=(test_input_iterator const& other);
    test_input_iterator& operator=(test_input_iterator&& other) noexcept;
    reference            operator*() const;
    pointer              operator->() const;
    test_input_iterator& operator++();
    test_input_iterator  operator++(int);

    friend bool          operator==(test_input_iterator const& lhs, test_input_iterator const& rhs)
    {
        return *lhs.m_underlying_iterator == *rhs.m_underlying_iterator;
    }
    friend bool operator!=(test_input_iterator const& lhs, test_input_iterator const& rhs)
    {
        return !(lhs == rhs);
    }

private:
    std::shared_ptr<Iterator> m_underlying_iterator;
};

template<class Iterator>
test_input_iterator<Iterator>::test_input_iterator(Iterator iterator)
    : m_underlying_iterator(std::make_shared<Iterator>(iterator))
{
}

template<class Iterator>
test_input_iterator<Iterator>::test_input_iterator(test_input_iterator const& copy)
    : m_underlying_iterator(copy.m_underlying_iterator)
{
}

template<class Iterator>
test_input_iterator<Iterator>::test_input_iterator(test_input_iterator&& other) noexcept
    : m_underlying_iterator(pw::move(other.m_underlying_iterator))
{
}

template<class Iterator>
test_input_iterator<Iterator>&
test_input_iterator<Iterator>::operator=(test_input_iterator const& other)
{
    if (this != &other)
    {
        m_underlying_iterator = other.m_underlying_iterator;
    }
    return *this;
}

template<class Iterator>
test_input_iterator<Iterator>&
test_input_iterator<Iterator>::operator=(test_input_iterator&& other) noexcept
{
    if (this != &other)
    {
        m_underlying_iterator = pw::move(other.m_underlying_iterator);
    }
    return *this;
}

template<class Iterator>
test_input_iterator<Iterator>::reference
test_input_iterator<Iterator>::operator*() const
{
    return *(*m_underlying_iterator);
}

template<class Iterator>
test_input_iterator<Iterator>::pointer
test_input_iterator<Iterator>::operator->() const
{
    return m_underlying_iterator->operator->();
}

template<class Iterator>
test_input_iterator<Iterator>&
test_input_iterator<Iterator>::operator++()
{
    ++(*m_underlying_iterator);
    return *this;
}

template<class Iterator>
test_input_iterator<Iterator>
test_input_iterator<Iterator>::operator++(int)
{
    test_input_iterator temp(*this);
    ++(*m_underlying_iterator);
    return temp;
}
} // namespace pw::test

#endif /* INCLUDED_PW_TEST_INPUT_ITERATOR_H */