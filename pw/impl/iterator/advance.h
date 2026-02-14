#ifndef INCLUDED_PW_IMPL_ADVANCE_H
#define INCLUDED_PW_IMPL_ADVANCE_H

#include <pw/impl/iterator/iterator_tag.h>
#include <pw/impl/iterator/iterator_traits.h>
#include <pw/impl/type_traits/is_same.h>

namespace pw {

/**
 * Advance an iterator by distance
 *
 * @tparam Iterator The type of iterator
 * @tparam Distance How far
 * @param iterator The iterator to change
 * @param n How far to move iterator
 */
template<class Iterator, class Distance>
constexpr void
advance(Iterator& iterator, Distance n)
{
    using iterator_category = iterator_traits<Iterator>::iterator_category;

    if constexpr (is_same<iterator_category, random_access_iterator_tag>::value)
    {
        iterator += n;
    }
    else if constexpr (is_same<iterator_category, bidirectional_iterator_tag>::value)
    {
        while (n > 0)
        {
            ++iterator;
            --n;
        }
        while (n < 0)
        {
            --iterator;
            ++n;
        }
    }
    else
    {
        while (n--)
        {
            ++iterator;
        }
    }
}

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_ADVANCE_H */
