#ifndef INCLUDED_PW_IMPL_DISTANCE_H
#define INCLUDED_PW_IMPL_DISTANCE_H

#include <pw/impl/iterator_traits.h>

#include <pw/impl/is_same.h>

namespace pw {

/**
 * @brief Calculates the number of elements between two iterators
 *
 * For random access iterators, this function performs end - begin in constant time.
 * For other iterator categories, it advances from begin to end and counts the steps,
 * resulting in linear time complexity.
 *
 * @tparam Iterator The iterator type
 *
 * @param begin Iterator to the first element
 * @param end Iterator to one past the last element
 *
 * @return The number of elements between begin and end
 *
 * @note For input iterators, this operation is destructive as it needs to increment
 *       the iterator to count the elements.
 */
template<class Iterator>
constexpr iterator_traits<Iterator>::difference_type
distance(Iterator begin, Iterator end)
{
    using iterator_category = iterator_traits<Iterator>::iterator_category;

    if constexpr (is_same<iterator_category, random_access_iterator_tag>::value)
    {
        return end - begin;
    }
    else
    {
        typename iterator_traits<Iterator>::difference_type d {};

        while (begin != end)
        {
            ++begin;
            ++d;
        }
        return d;
    }
}

} // namespace pw

#endif /* INCLUDED_PW_IMPL_DISTANCE_H
 */
