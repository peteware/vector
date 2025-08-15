#ifndef INCLUDED_PW_IMPL_PREV_H
#define INCLUDED_PW_IMPL_PREV_H

#include <pw/impl/advance.h>
#include <pw/impl/iterator_traits.h>

namespace pw {

template<class Iterator>
constexpr Iterator
prev(Iterator iterator, typename iterator_traits<Iterator>::difference_type n = 1)
{
    advance(iterator, -n);
    return iterator;
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_PREV_H */
