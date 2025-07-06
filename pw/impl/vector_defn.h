#ifndef PW_IMPL_VECTOR_DEFN_H
#define PW_IMPL_VECTOR_DEFN_H

#include "pw/internal/unimplemented.h"

#include <pw/impl/vector_decl.h>

#include <pw/impl/copy.h>
#include <pw/impl/distance.h>
#include <pw/impl/min.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_default_construct.h>
#include <pw/impl/uninitialized_fill.h>
#include <pw/impl/uninitialized_move.h>

namespace pw {
template<class Type>
Type&
makeReference()
{
    static Type s_value;

    return s_value;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector() noexcept(noexcept(allocator_type()))
    : m_storage(allocator_type())
{
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(allocator_type const& alloc) noexcept
    : m_storage(alloc)
{
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type             count,
                                          value_type const&     value,
                                          allocator_type const& alloc)
    : m_storage(alloc)
{
    auto lambda = [count = count, value = value](pointer dest) -> void {
        uninitialized_fill(dest, dest + count, value);
    };
    m_storage.reserve(count, lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type             count,
                                          allocator_type const& alloc)
    : m_storage(alloc)
{
    auto lambda = [count = count](pointer dest) -> void {
        uninitialized_default_construct(dest, dest + count);
    };
    m_storage.reserve(count, lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy)
    : m_storage(allocator_type())
{
    auto lambda = [begin = copy.begin(),
                   end   = copy.end()](pointer dest) -> void {
        uninitialized_copy(begin, end, dest);
    };
    m_storage.reserve(copy.size(), lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const&         copy,
                                          allocator_type const& alloc)
    : m_storage(alloc)
{
    auto lambda = [begin = copy.begin(),
                   end   = copy.end()](pointer dest) -> void {
        uninitialized_copy(begin, end, dest);
    };
    m_storage.reserve(copy.size(), lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& other) noexcept
    : m_storage(allocator_type())
{
    auto lambda = [begin = other.begin(),
                   end   = other.end()](pointer dest) -> void {
        uninitialized_move(begin, end, dest);
    };
    m_storage.reserve(other.size(), lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&&         other,
                                          const Allocator& alloc)
    : m_storage(alloc)
{
    auto lambda = [begin = other.begin(),
                   end   = other.end()](pointer dest) -> void {
        uninitialized_move(begin, end, dest);
    };
    m_storage.reserve(other.size(), lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(pw::initializer_list<value_type> init,
                                          allocator_type const& alloc)
    : m_storage(alloc)
{
    auto lambda = [begin = init.begin(),
                   end   = init.end()](pointer dest) -> void {
        uninitialized_copy(begin, end, dest);
    };
    m_storage.reserve(init.size(), lambda);
}

template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::vector(Iterator              first,
                                          Iterator              last,
                                          allocator_type const& alloc)
    : m_storage(alloc)
{
    size_type count  = distance(first, last);

    auto      lambda = [begin = first, end = last](pointer dest) -> void {
        uninitialized_copy(begin, end, dest);
    };
    m_storage.reserve(count, lambda);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::~vector()
{
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::swap(vector& other) noexcept(
    pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
    pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    m_storage.swap(other.m_storage);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(const vector& other)
{
    if constexpr (allocator_traits<allocator_type>::
                      propagate_on_container_copy_assignment::value)
    {
        // TODO: Check if same allocator.  If so, we should still
        //       copy the allocator however we do not need to reallocate
        //       the storage if there is enough space.
        Storage storage { other.get_allocator() };

        storage.reserve(other.size());
        uninitialized_copy(other.begin(), other.end(), storage.begin());
        storage.set_size(other.size());
        m_storage.swap(storage);
    }
    else if (m_storage.allocated() < other.size())
    {
        Storage storage { allocator_type() };

        storage.reserve(other.size());
        uninitialized_copy(other.begin(), other.end(), storage.begin());
        storage.set_size(other.size());
        m_storage = storage;
    }
    else
    {
        size_type initsize = min(m_storage.size(), other.size());
        copy(other.begin(), other.begin() + initsize, m_storage.begin());
        uninitialized_copy(other.begin() + initsize,
                           other.end(),
                           m_storage.begin() + initsize);
    }
    return *this;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(pw::initializer_list<value_type> init_list)
{
    (void)init_list;
    throw internal::Unimplemented(__func__);
    //return *this;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(vector&& other) noexcept(
    pw::allocator_traits<
        allocator_type>::propagate_on_container_move_assignment::value ||
    pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    (void)other;
    throw internal::Unimplemented(__func__);
    //return *this;
}

template<class Type, class Allocator>
template<class Iterator>
constexpr void
vector<Type, Allocator>::assign(Iterator begin, Iterator end)
{
    (void)begin;
    (void)end;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(size_type count, value_type const& value)
{
    (void)count;
    (void)value;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(pw::initializer_list<value_type> init_list)
{
    (void)init_list;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::allocator_type
vector<Type, Allocator>::get_allocator() const
{
    return m_storage.get_allocator();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::reference
vector<Type, Allocator>::at(size_type position)
{
    (void)position;
    throw internal::Unimplemented(__func__);
    // return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reference
vector<Type, Allocator>::at(size_type position) const
{
    (void)position;
    throw internal::Unimplemented(__func__);
    //return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::reference
vector<Type, Allocator>::operator[](size_type position)
{
    return *(m_storage.begin() + position);
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reference
vector<Type, Allocator>::operator[](size_type position) const
{
    (void)position;
    throw internal::Unimplemented(__func__);
    //return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::reference
vector<Type, Allocator>::front()
{
    throw internal::Unimplemented(__func__);
    //return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reference
vector<Type, Allocator>::front() const
{
    throw internal::Unimplemented(__func__);
    // return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::reference
vector<Type, Allocator>::back()
{
    throw internal::Unimplemented(__func__);
    // return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reference
vector<Type, Allocator>::back() const
{
    throw internal::Unimplemented(__func__);
    // return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr Type*
vector<Type, Allocator>::data() noexcept
{
    throw internal::Unimplemented(__func__);
    // return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr Type const*
vector<Type, Allocator>::data() const noexcept
{
    throw internal::Unimplemented(__func__);
    // return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::begin() noexcept
{
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::begin() const noexcept
{
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::end() noexcept
{
    return m_storage.end();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::end() const noexcept
{
    return m_storage.end();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cbegin() const noexcept
{
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cend() const noexcept
{
    return m_storage.end();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr bool
vector<Type, Allocator>::empty() const noexcept
{
    return m_storage.empty();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::size_type
vector<Type, Allocator>::size() const noexcept
{
    return m_storage.size();
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::size_type
vector<Type, Allocator>::max_size() const noexcept
{
    return 1;
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::size_type
vector<Type, Allocator>::capacity() const noexcept
{
    return m_storage.capacity();
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::shrink_to_fit()
{
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::reserve(size_type count)
{
    (void)count;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::clear() noexcept
{
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(const_reference value)
{
    (void)value;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(value_type&& value)
{
    (void)value;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type count)
{
    (void)count;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type count, const_reference value)
{
    (void)count;
    (void)value;
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::erase(const_iterator position)
{
    return erase(position, position + 1);
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::erase(const_iterator begin, const_iterator end)
{
    (void)begin;
    (void)end;
    throw internal::Unimplemented(__func__);
    // return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::pop_back()
{
    throw internal::Unimplemented(__func__);
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, const_reference value)
{
    return insert(position, static_cast<size_type>(1), value);
}

template<class Type, class Allocator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator  position,
                                size_type       count,
                                const_reference value)
{
    (void)position;
    (void)count;
    (void)value;
    throw internal::Unimplemented(__func__);
    // return &makeReference<value_type>();
}

template<class Type, class Allocator>
template<class Iterator>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position,
                                Iterator       first,
                                Iterator       last)
{
    (void)position;
    (void)first;
    (void)last;
    throw internal::Unimplemented(__func__);
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
template<class... Args>
constexpr typename vector<Type, Allocator>::reference
vector<Type, Allocator>::emplace_back(Args&&... args)
{
    throw internal::Unimplemented(__func__);
    // return makeReference<value_type>();
}

template<class Type, class Allocator>
template<class... Args>
constexpr typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::emplace(const_iterator position, Args&&... args)
{
    (void)position;
    throw internal::Unimplemented(__func__);
    // return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr void
swap(vector<Type, Allocator>& op1,
     vector<Type, Allocator>& op2) noexcept(noexcept(op1.swap(op2)))
{
    op1.swap(op2);
}

template<class Type, class Allocator>
constexpr bool
operator==(const pw::vector<Type, Allocator>& op1,
           const pw::vector<Type, Allocator>& op2)
{
    if (op1.size() != op2.size())
    {
        return false;
    }
    return (op1 <=> op2) == 0;
}

template<class Type, class Allocator>
constexpr auto
operator<=>(const pw::vector<Type, Allocator>& op1,
            const pw::vector<Type, Allocator>& op2)
    -> decltype(op1[0] <=> op2[0])
{
    throw internal::Unimplemented(__func__);
    // return op1[0] <=> op2[0];
}
} // namespace pw
#endif /* PW_IMPL_VECTOR_DEFN_H */
