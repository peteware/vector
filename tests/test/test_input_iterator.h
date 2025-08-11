#ifndef INCLUDED_PW_TEST_INPUT_ITERATOR_H
#define INCLUDED_PW_TEST_INPUT_ITERATOR_H

#include "pw/impl/move.h"

#include <memory>
#include <pw/impl/iterator_tag.h>
#include <pw/impl/iterator_traits.h>

namespace pw::test {

template<class Iterator>
struct test_input_iterator
{
    using iterator_category = pw::input_iterator_tag;
    using value_type        = pw::iterator_traits<Iterator>::value_type;
    using difference_type   = pw::iterator_traits<Iterator>::difference_type;
    using pointer           = pw::iterator_traits<Iterator>::pointer;
    using reference         = pw::iterator_traits<Iterator>::reference;

    test_input_iterator()   = default;
    explicit test_input_iterator(Iterator iterator)
        : m_underlying_iterator(std::make_shared<Iterator>(iterator))
    {
    }
    test_input_iterator(const test_input_iterator& copy)
        : m_underlying_iterator(copy.m_underlying_iterator)
    {
    }
    test_input_iterator(test_input_iterator&& other) noexcept
        : m_underlying_iterator(pw::move(other.m_underlying_iterator))
    {
    }
    ~test_input_iterator() = default;
    test_input_iterator& operator=(const test_input_iterator& other)
    {
        if (this != &other)
        {
            m_underlying_iterator = other.m_underlying_iterator;
        }
        return *this;
    }
    test_input_iterator& operator=(test_input_iterator&& other) noexcept
    {
        if (this != &other)
        {
            m_underlying_iterator = pw::move(other.m_underlying_iterator);
        }
        return *this;
    }
    reference            operator*() const { return *(*m_underlying_iterator); }
    pointer              operator->() const { return m_underlying_iterator; }

    test_input_iterator& operator++()
    {
        ++(*m_underlying_iterator);
        return *this;
    }

    test_input_iterator operator++(int)
    {
        test_input_iterator temp(*this);
        ++(*m_underlying_iterator);
        return temp;
    }

    friend bool operator==(const test_input_iterator& lhs, const test_input_iterator& rhs)
    {
        return *lhs.m_underlying_iterator == *rhs.m_underlying_iterator;
    }

    friend bool operator!=(const test_input_iterator& lhs, const test_input_iterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    std::shared_ptr<Iterator> m_underlying_iterator;
};

} // namespace pw::test

#endif /* INCLUDED_PW_TEST_INPUT_ITERATOR_H */