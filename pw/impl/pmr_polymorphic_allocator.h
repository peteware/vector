#ifndef INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H
#define INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H

namespace pw::pmr {
template<class Type>
class polymorphic_allocator
{
public:
    using value_type = Type;
};
} // namespace pw::pmr

#endif /*   INCLUDED_PW_PMR_POLYMORPHIC_ALLOCATOR_H */
