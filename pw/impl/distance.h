#ifndef INCLUDED_PW_IMPL_DISTANCE_H
#define INCLUDED_PW_IMPL_DISTANCE_H

#include <pw/impl/iterator_traits.h>

namespace pw {

template<class Iterator>
constexpr typename pw::iterator_traits<Iterator>::difference_type
distance(Iterator begin, Iterator end)
{
    typename pw::iterator_traits<Iterator>::difference_type d {};

    while (begin++ != end)
    {
        ++d;
    }
    return d;
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_DISTANCE_H
 */
