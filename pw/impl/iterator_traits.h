#ifndef INCLUDED_PW_IMPL_ITERATOR_TRAITS_H
#define INCLUDED_PW_IMPL_ITERATOR_TRAITS_H

template<class Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

template<class Type>
struct iterator_traits<Type*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef Type                       value_type;
    typedef ptrdiff_t                  difference_type;
    typedef Type*                      pointer;
    typedef Type&                      reference;
};
#endif /*  INCLUDED_PW_IMPL_ITERATOR_TRAITS_H */
