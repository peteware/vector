#ifndef INCLUDED_PW_IMPL_FILL_N_H
#define INCLUDED_PW_IMPL_FILL_N_H

namespace pw {
template<class Iterator, class Size, class Type>
constexpr Iterator
fill_n(Iterator first, Size count, Type const& value)
{
    for (Size i = 0; i < count; ++i)
    {
        *first++ = value;
    }
    return first;
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_FILL_N_H */
