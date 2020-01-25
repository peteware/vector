#ifndef _INCLUDED_PW_IMPL_FILL_H_
#define _INCLUDED_PW_IMPL_FILL_H_

namespace pw {

template<class ForwardIterator, class Type>
void
fill(ForwardIterator begin, ForwardIterator end, Type const& value)
{
    while (begin != end)
    {
        *begin++ = value;
    }
}

} // namespace pw
#endif /*  _INCLUDED_PW_IMPL_FILL_H_ */
