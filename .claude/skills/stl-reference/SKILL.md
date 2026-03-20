---
name: stl-reference
description: Background context for STL implementation work
user-invocable: false
---

When working on STL implementations in this project:

1. **Standard Compliance**: Verify behavior against cppreference.com for the standard specification
2. **Exception Safety**: Match the standard's exception safety guarantees (basic, strong, nothrow)
3. **Constexpr**: Ensure constexpr compatibility where C++23 requires it
4. **Allocator Awareness**: Follow allocator-aware container requirements including:
   - Proper use of allocator_traits
   - POCCA, POCMA, POCS semantics
   - Allocator propagation on copy/move/swap
5. **Iterator Invalidation**: Document and implement correct iterator invalidation rules
6. **Complexity Guarantees**: Meet the standard's complexity requirements

Use web search to look up cppreference when uncertain about standard behavior.