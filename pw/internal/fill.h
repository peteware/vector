#ifndef _INCLUDED_PW_INTERNAL_FILL_H_
#define _INCLUDED_PW_INTERNAL_FILL_H_

namespace pw { namespace internal {

template<class ForwardIterator, class Type>
void
fill(ForwardIterator begin, ForwardIterator end, Type const& value)
{
    while (begin != end)
    {
        *begin++ = value;
    }
}

}} // namespace pw::internal
#endif /*  _INCLUDED_PW_INTERNAL_FILL_H_ */
