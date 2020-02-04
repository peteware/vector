target_sources(pw INTERFACE
    allocator
    cstddef
    memory
    memory_resource
    type_traits
    vector
    impl/allocator.h
    impl/allocator_traits.h
    impl/bool_type.h
    impl/integral_constant.h
    impl/is_class.h
    impl/is_empty.h
    impl/is_same.h
    impl/make_unsigned.h
    impl/ptrdiff.h
    impl/size.h
    impl/void.h
    internal/allocate.h
    internal/destroy.h
    internal/extract_or.h
    internal/meta.h
    
)
