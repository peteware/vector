target_sources(pw INTERFACE
    algorithm
    allocator
    cstddef
    memory
    memory_resource
    type_traits
    utility
    vector

    impl/allocator.h
    impl/allocator_traits.h
    impl/bool_type.h
    impl/copy.h
    impl/fill.h
    impl/for_each.h
    impl/integral_constant.h
    impl/is_class.h
    impl/is_empty.h
    impl/is_same.h
    impl/make_unsigned.h
    impl/max.h
    impl/pmr_polymorphic_allocator.h
    impl/pointer_traits.h
    impl/ptrdiff.h
    impl/size.h
    impl/swap.h
    impl/void.h
    internal/check_empty.h
    internal/destroy.h
    internal/extract_or.h
    internal/meta.h
    
)
