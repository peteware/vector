#ifndef INCLUDED_PW_IMPL_NEXT_H
#define INCLUDED_PW_IMPL_NEXT_H

#include <pw/impl/advance.h>
#include <pw/impl/iterator_traits.h>

namespace pw {

template<class Iterator>
constexpr Iterator
next(Iterator                                                iterator,
     typename pw::iterator_traits<Iterator>::difference_type n = 1)
{
    pw::advance(iterator, n);
    return iterator;
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_NEXT_H */
