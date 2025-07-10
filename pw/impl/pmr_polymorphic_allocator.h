#ifndef _INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H
#define _INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H

namespace pw::pmr {
template<class Type>
class polymorphic_allocator
{
public:
    using value_type = Type;
};
} // namespace pw::pmr

#endif /*  _INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H */
