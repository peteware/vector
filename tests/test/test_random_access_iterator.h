#ifndef INCLUDED_PW_TEST_RANDOM_ACCESS_ITERATOR_H
#define INCLUDED_PW_TEST_RANDOM_ACCESS_ITERATOR_H

#include <pw/impl/iterator/iterator_tag.h>
#include <tests/test/test_opcounter.h>

namespace pw::test {

template<class Iterator>
struct random_access_iterator
{
    using iterator_category = random_access_iterator_tag;
    using value_type        = iterator_traits<Iterator>::value_type;
    using difference_type   = iterator_traits<Iterator>::difference_type;
    using pointer           = iterator_traits<Iterator>::pointer;
    using reference         = iterator_traits<Iterator>::reference;

    static OpCounter& getCounter() { return s_opCounter; }

    random_access_iterator() { s_opCounter.addDefaultConstructor(); }

    explicit random_access_iterator(Iterator iterator)
        : m_iterator(iterator)
    {
        s_opCounter.addDefaultConstructor();
    }

    random_access_iterator(random_access_iterator const& copy)
        : m_iterator(copy.m_iterator)
    {
        s_opCounter.addCopyConstructor();
    }

    random_access_iterator(random_access_iterator&& op2) noexcept
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

#endif /* INCLUDED_PW_TEST_RANDOM_ACCESS_ITERATOR_H */
