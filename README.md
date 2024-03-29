# Example implementation of std::vector

[![CMake](https://github.com/peteware/vector/actions/workflows/cmake.yml/badge.svg)](https://github.com/peteware/vector/actions/workflows/cmake.yml)

## Understanding STL

I want to understand the Standard Template Library (STL) in excruciating detail and
the best way is to implement it.  I'm starting off with `std::vector` as it is
relatively simple algorithmitically but a fundamental container.

This is not intended for production use.  It is targetting C++-17 with
a little bit of C++-20.  There's no attempt to be portable or backward
compatible beyond following good practices.  There's no obfuscating
names with `__` to avoid conflicts with other code.

Ideally, no `std` code is used (see exceptions below).

The namespace is `pw` and include files are `pw/<name>`.  Most of
the actual implementations are in `pw/impl/<file.h>`.  In fact,
most of the STL include files consist primarily of including various
implementation files.

The namespace `pw::internal` is for private details and
are in `pw/internal/<file.h>`.  These should not be used by other
code.

Unit tests are in `tests/unit/<file.t.cpp>` and use the
[catch2](https://github.com/catchorg/Catch2) framework.

## Things I didn't implement

- `<limits>` and `<stdexcept>` are used from `std` because it seems like a lot of work
  for not much gain.
- `std::initializer_list` is inherently a compiler builtin struct
- `is_class` used builtin `__is_class(Type)`.  Because I couldn't implement a working version.
- `is_union` used builtin `__is_union(Type)`.  ditto.
- `is_nothrow_constructible` used builtin `__is_nothrow_constructible(Type, Args...)`.  ditto

## ToDo

### Unit tests

Need the following unit tests to be written:

- [ ] `vector::vector((vector&& other, const Allocator& alloc)`
- [ ] `vector::shrink_to_fit()`
- [ ] `vector::reserve()` 
- [ ] `vector::::operator=(vector&& other) noexcept` when different
