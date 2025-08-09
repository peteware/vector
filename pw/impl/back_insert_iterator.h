#ifndef INCLUDED_IMPL_BACK_INSERT_ITERATOR_H
#define INCLUDED_IMPL_BACK_INSERT_ITERATOR_H

#include <pw/impl/iterator_tag.h>
#include <pw/impl/move.h>

namespace pw {

template<class Container>
class back_insert_iterator
{
public:
    using iterator_category = pw::output_iterator_tag;
    using value_type        = void;
    using difference_type   = void;
    using pointer           = void;
    using reference         = void;
    using container_type    = Container;

    explicit back_insert_iterator(container_type& container);

    back_insert_iterator<Container>& operator=(const Container::value_type& value);
    back_insert_iterator<Container>& operator=(Container::value_type&& value);
    back_insert_iterator<Container>& operator*();
    back_insert_iterator<Container>& operator++();
    back_insert_iterator<Container>& operator++(int);

protected:
    container_type* m_container;
};

template<class Container>
back_insert_iterator<Container>::back_insert_iterator(container_type& container)
    : m_container(&container)
{
}

template<class Container>
back_insert_iterator<Container>&
back_insert_iterator<Container>::operator=(const typename Container::value_type& value)
{
    m_container->push_back(value);
    return *this;
}

template<class Container>
back_insert_iterator<Container>&
back_insert_iterator<Container>::operator=(typename Container::value_type&& value)
{
    m_container->push_back(pw::move(value));
    return *this;
}

template<class Container>
back_insert_iterator<Container>&
back_insert_iterator<Container>::operator*()
{
    return *this;
}

template<class Container>
back_insert_iterator<Container>&
back_insert_iterator<Container>::operator++()
{
    return *this;
}

template<class Container>
back_insert_iterator<Container>&
back_insert_iterator<Container>::operator++(int)
{
    return *this;
}

} // namespace pw

#endif /* INCLUDED_IMPL_BACK_INSERT_ITERATOR_H */
