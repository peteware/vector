#ifndef INCLUDED_PW_INTERNAL_MAKE_UNSIGNED_H
#define INCLUDED_PW_INTERNAL_MAKE_UNSIGNED_H

namespace pw { namespace internal {
template<typename Type>
struct make_unsigned
{
    using type = Type;
};

template<>
struct make_unsigned<signed char>
{
    using type = unsigned char;
};

template<>
struct make_unsigned<int>
{
    using type = unsigned int;
};

template<>
struct make_unsigned<long>
{
    using type = unsigned long;
};

template<>
struct make_unsigned<long long>
{
    using type = unsigned long long;
};

}} // End of namespace pw::internal
#endif /* INCLUDED_PW_INTERNAL_MAKE_UNSIGNED_H */
