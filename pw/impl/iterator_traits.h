#ifndef INCLUDED_PW_IMPL_ITERATOR_TRAITS_H
#define INCLUDED_PW_IMPL_ITERATOR_TRAITS_H

#include <pw/impl/iterator_tag.h>
#include <pw/impl/ptrdiff.h>

namespace pw {

template<class Iterator>
struct iterator_traits
{
    using iterator_category = Iterator::iterator_category;
    using value_type        = Iterator::value_type;
    using difference_type   = Iterator::difference_type;
    using pointer           = Iterator::pointer;
    using reference         = Iterator::reference;
};

template<class Type>
struct iterator_traits<Type*>
{
    using iterator_category = pw::random_access_iterator_tag;
    using value_type        = Type;
    using difference_type   = ptrdiff_t;
    using pointer           = Type*;
    using reference         = Type&;
};
} // namespace pw

#endif /*  INCLUDED_PW_IMPL_ITERATOR_TRAITS_H */
