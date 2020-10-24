#ifndef INCLUDED_PW_IMPL_DISTANCE_H
#define INCLUDED_PW_IMPL_DISTANCE_H

#include <pw/impl/iterator_traits.h>

#include <pw/impl/is_same.h>

namespace pw {

template<class Iterator>
constexpr typename pw::iterator_traits<Iterator>::difference_type
distance(Iterator begin, Iterator end)
{
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;

    if constexpr (is_same<iterator_category, random_access_iterator_tag>::value)
    {
        return end - begin;
    }
    else
    {
        typename pw::iterator_traits<Iterator>::difference_type d {};

        while (begin++ != end)
        {
            ++d;
        }
        return d;
    }
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_DISTANCE_H
 */
