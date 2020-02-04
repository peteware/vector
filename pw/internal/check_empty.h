#ifndef INCLUDED_PW_INTERNAL_CHECK_EMPTY_H
#define INCLUDED_PW_INTERNAL_CHECK_EMPTY_H

namespace pw { namespace internal {

template<class Type>
struct check_empty1 : public Type
{
    double m_dummy;
};

struct check_empty2
{
    double m_dummy;
};
}} // namespace pw::internal

#endif /*  INCLUDED_PW_INTERNAL_CHECK_EMPTY_H */
