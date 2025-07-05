#ifndef INCLUDED_TEST_RANDOM_ACCESS_ITERATOR_H
#define INCLUDED_TEST_RANDOM_ACCESS_ITERATOR_H

#include <pw/impl/iterator_tag.h>
#include <test_opcounter.h>

namespace pw::test {

template<class Iterator>
struct random_access_iterator
{
    using iterator_category = pw::random_access_iterator_tag;
    using value_type = typename pw::iterator_traits<Iterator>::value_type;
    using difference_type =
        typename pw::iterator_traits<Iterator>::difference_type;
    using pointer   = typename pw::iterator_traits<Iterator>::pointer;
    using reference = typename pw::iterator_traits<Iterator>::reference;

    static OpCounter getCounter() { return s_opCounter; }

    random_access_iterator() { s_opCounter.addDefaultConstructor(); }

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

    ~random_access_iterator() { s_opCounter.addDestructor(); }

private:
    Iterator         m_iterator = {};
    static OpCounter s_opCounter;
};

} // namespace pw::test

#endif /* INCLUDED_TEST_RANDOM_ACCESS_ITERATOR_H */
