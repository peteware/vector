#ifndef INCLUDED_PW_IMPL_COPY_H
#define INCLUDED_PW_IMPL_COPY_H

namespace pw {

/**
 * @brief Copies elements from one range to another.
 *
 * Copies the elements in the range [begin, end) to the range beginning at dest.
 * The assignment is performed using operator= for each element.
 *
 * @tparam InputIterator Iterator type for the input range.
 * @tparam OutputIterator Iterator type for the output range.
 * @param begin Iterator to the beginning of the input range.
 * @param end Iterator to the end of the input range.
 * @param dest Iterator to the beginning of the destination range.
 * @return OutputIterator Iterator to the element past the last element copied.
 *
 * @note The source and destination ranges must not overlap.
 */
template<class InputIterator, class OutputIterator>
OutputIterator
copy(InputIterator begin, InputIterator end, OutputIterator dest)
{
    while (begin != end)
    {
        *dest++ = *begin++;
    }
    return dest;
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_COPY_H */
