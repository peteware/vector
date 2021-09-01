#ifndef INCLUDED_PW_IMPL_COPY_H
#define INCLUDED_PW_IMPL_COPY_H

namespace pw {

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
#endif /*  INCLUDED_PW_COPY_H */
