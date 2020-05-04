#ifndef INCLUDED_TESTS_RANDOM_ACCESS_ITERATOR_H
#define INCLUDED_TESTS_RANDOM_ACCESS_ITERATOR_H

#include "test_opcounter.h"
#include <pw/impl/iterator_tag.h>

namespace pw { namespace test {

template<class Iterator>
struct random_access_iterator
{
    using iterator_category = pw::random_access_iterator_tag;
    using value_type        = typename pw::iterator_traits<Iterator>::value_type;
    using difference_type   = typename pw::iterator<Iterator>::difference_type;
    using pointer           = typename pw::iterator<Iterator>::pointer;
    using reference         = typename pw::iterator<Iterator>::reference;

    static OpCounter getCounter()
    {
        return s_opCounter;
    }

    random_access_iterator()
    {
        s_opCounter.addDefaultConstructor();
    }

    explicit random_access_iterator(Iterator iterator)
        : m_iterator(iterator)
    {
        s_opCounter.addDefaultConstructor();
    }

    random_access_iterator(const random_access_iterator& copy)
        : m_iterator(copy.m_iterator)
    {
        s_opCounter.addCopyConstructor();
    }

    random_access_iterator(random_access_iterator&& op2)
        : m_iterator(op2.m_iterator)
    {
        s_opCounter.addMoveConstructor();
    }

    ~random_access_iterator()
    {
        s_opCounter.addDestructor();
    }

private:
    Iterator         m_iterator = {};
    static OpCounter s_opCounter;
};

}} // namespace pw::test

#endif /* INCLUDED_TESTS_RANDOM_ACCESS_ITERATOR_H */
