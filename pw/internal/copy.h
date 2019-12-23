#ifndef INCLUDED_PW_INTERNAL_COPY_H
#define INCLUDED_PW_INTERNAL_COPY_H
namespace pw { namespace internal {

template<class InputIterator, class OutputIterator>
OutputIterator
copy(InputIterator begin, InputIterator end, OutputIterator dest)
{
    while (begin != end)
    {
        *dest = *begin++;
    }
    return dest;
}

}} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_COPY_H */
