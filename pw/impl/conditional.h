#ifndef INCLUDED_PW_IMPL_CONDITIONAL_H
#define INCLUDED_PW_IMPL_CONDITIONAL_H

namespace pw {
template<bool B, class TrueType, class FalseType>
struct conditional
{
    using type = TrueType;
};

template<class TrueType, class FalseType>
struct conditional<false, TrueType, FalseType>
{
    using type = FalseType;
};
} // namespace pw

#endif /* INCLUDED_PW_IMPL_CONDITIONAL_H */
