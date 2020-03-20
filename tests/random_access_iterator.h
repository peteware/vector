#ifndef INCLUDED_TESTS_RANDOM_ACCESS_ITERATOR_H
#define INCLUDED_TESTS_RANDOM_ACCESS_ITERATOR_H

#include "conscounter.h"
#include <pw/impl/iterator_tag.h>

template<class Iterator>
struct random_access_iterator
{
    using iterator_category = pw::random_access_iterator_tag;
    using value_type        = typename pw::iterator_traits<Iterator>::value_type;
    using difference_type   = typename pw::iterator<Iterator>::difference_type;
    using pointer           = typename pw::iterator<Iterator>::pointer;
    using reference         = typename pw::iterator<Iterator>::reference;

    static ConsCounter getCounter()
    {
        return s_consCounter;
    }

    random_access_iterator()
    {
        s_consCounter.addDefaultConstructor();
    }

    explicit random_access_iterator(Iterator iterator)
        : m_iterator(iterator)
    {
        s_consCounter.addDefaultConstructor();
    }

    random_access_iterator(const random_access_iterator& copy)
        : m_iterator(copy.m_iterator)
    {
        s_consCounter.addCopyConstructor();
    }

    random_access_iterator(random_access_iterator&& op2)
        : m_iterator(op2.m_iterator)
    {
        s_consCounter.addMoveConstructor();
    }

    ~random_access_iterator()
    {
        s_consCounter.addDestructor();
    }

private:
    Iterator           m_iterator = {};
    static ConsCounter s_consCounter;
};

#endif /* INCLUDED_TESTS_RANDOM_ACCESS_ITERATOR_H */
