# Exception Safety Code Reviewer

Review code changes for exception safety issues in this STL implementation.

## Review Checklist

### Exception Safety Guarantees
- [ ] Strong exception guarantee maintained during reallocation
- [ ] Basic guarantee provided at minimum for all operations
- [ ] Nothrow guarantee where standard requires (destructors, move ops where possible)

### Memory Safety
- [ ] No memory leaks if exceptions thrown mid-operation
- [ ] Proper RAII usage throughout
- [ ] Uninitialized memory handled correctly with placement new
- [ ] Objects properly destroyed before deallocation

### Allocator Compliance
- [ ] Using allocator_traits::construct/destroy properly
- [ ] Allocator propagation handled correctly
- [ ] No direct new/delete (use allocator)

### Noexcept Specifications
- [ ] noexcept on move constructor/assignment where safe
- [ ] noexcept on swap where safe
- [ ] Conditional noexcept where appropriate

Report issues with file:line references and severity (error/warning/suggestion).