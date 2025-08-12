#ifndef INCLUDED_PW_IMPL_VECTOR_DEFN_H
#define INCLUDED_PW_IMPL_VECTOR_DEFN_H

#include "fill_n.h"
#include "is_base_of.h"
#include "move_backward.h"

#include <pw/impl/vector_decl.h>

#include "pw/internal/unimplemented.h"
#include <pw/impl/copy.h>
#include <pw/impl/destroy.h>
#include <pw/impl/distance.h>
#include <pw/impl/min.h>
#include <pw/impl/move_alg.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_default_construct.h>
#include <pw/impl/uninitialized_fill.h>
#include <pw/impl/uninitialized_move.h>

namespace pw {

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
    : m_storage(alloc, count)
{
    pw::uninitialized_fill(m_storage.begin(), m_storage.begin() + count, value);
    m_storage.set_size(count);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type count, allocator_type const& alloc)
    : m_storage(alloc, count)
{
    pw::uninitialized_default_construct(m_storage.begin(), m_storage.begin() + count);
    m_storage.set_size(count);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy)
    : m_storage(allocator_type(), copy.size())
{
    pw::uninitialized_copy(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy, allocator_type const& alloc)
    : m_storage(alloc, copy.size())
{
    pw::uninitialized_copy(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& copy) noexcept
    : m_storage(allocator_type(), copy.size())
{
    pw::uninitialized_move(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& copy, const Allocator& alloc)
    : m_storage(alloc, copy.size())
{
    pw::uninitialized_move(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(pw::initializer_list<value_type> init, allocator_type const& alloc)
    : m_storage(alloc, init.size())
{
    pw::uninitialized_copy(init.begin(), init.end(), m_storage.begin());
    m_storage.set_size(init.size());
}

template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::vector(Iterator first, Iterator last, allocator_type const& alloc)
    : m_storage(alloc)
{
    if constexpr (pw::is_base_of<pw::forward_iterator_tag, Iterator>::value)
    {
        size_type count  = pw::distance(first, last);

        auto      lambda = [begin = first, end = last](pointer dest) -> void {
            pw::uninitialized_copy(begin, end, dest);
        };
        m_storage.reserve(count, lambda);
    }
    else
    {
        // This is an input iterator so we need to grow as we go
        while (first != last)
        {
            push_back(*first);
            ++first;
        }
    }
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::swap(vector& other)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    m_storage.swap(other.m_storage, allocator_traits<allocator_type>::propagate_on_container_swap::value);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(const vector& other)
{
    if constexpr (allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
    {
        // TODO: Check if same allocator.  If so, we should still
        //       copy the allocator however we do not need to reallocate
        //       the storage if there is enough space.
        Storage tmp { other.get_allocator(), other.size() };

        pw::uninitialized_copy(other.begin(), other.end(), tmp.begin());
        tmp.set_size(other.size());
        m_storage.swap(tmp, true);
    }
    else if (m_storage.size() < other.size())
    {
        if (m_storage.allocated() >= other.size())
        {
            pw::copy(other.begin(), other.begin() + m_storage.size(), m_storage.begin());
            pw::uninitialized_copy(other.begin() + m_storage.size(), other.end(), m_storage.end());
        }
        else
        {
            // Not enough space so we need to reallocate
            Storage tmp { allocator_type(), other.size() };
            pw::uninitialized_copy(other.begin(), other.end(), tmp.begin());
            tmp.set_size(other.size());
            m_storage.swap(tmp, false);
        }
        m_storage.set_size(other.size());
    }
    else if (m_storage.size() == other.size())
    {
        pw::copy(other.begin(), other.end(), m_storage.begin());
    }
    else // m_storage.size() > other.size()
    {
        pw::copy(other.begin(), other.end(), m_storage.begin());
        pw::destroy(m_storage.begin() + other.size(), m_storage.end());
        m_storage.set_size(other.size());
    }
    return *this;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(pw::initializer_list<value_type> init_list)
{
    Storage tmp { allocator_type(), init_list.size() };

    pw::uninitialized_copy(init_list.begin(), init_list.end(), tmp.begin());
    tmp.set_size(init_list.size());
    m_storage.swap(tmp, false);
    return *this;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(vector&& other)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    if constexpr (allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)
    {
        Storage storage { other.get_allocator(), other.size() };
        pw::uninitialized_copy(other.begin(), other.end(), storage.begin());
        storage.set_size(other.size());
        m_storage.swap(storage, true);
    }
    else
    {
        size_type init_size = min(m_storage.size(), other.size());
        move(other.begin(), other.begin() + init_size, m_storage.begin());
        if (size() < other.size())
        {
            pw::uninitialized_move(other.begin() + init_size, other.end(), m_storage.begin() + init_size);
        }
        else
        {
            pw::destroy(m_storage.begin() + init_size, m_storage.end());
        }
        m_storage.set_size(other.size());
    }
    return *this;
}

template<class Type, class Allocator>
template<class Iterator>
constexpr void
vector<Type, Allocator>::assign(Iterator begin, Iterator end)
{
    if constexpr (is_base_of<pw::forward_iterator_tag, Iterator>::value)
    {
        size_type count = pw::distance(begin, end);
        Storage   tmp { allocator_type(), count };

        pw::uninitialized_copy(begin, end, tmp.begin());
        tmp.set_size(count);
        m_storage.swap(tmp, false);
    }
    else
    {
        m_storage.reset();
        while (begin != end)
        {
            push_back(*begin);
            ++begin;
        }
    }
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(size_type count, value_type const& value)
{
    Storage tmp { allocator_type(), count };
    pw::uninitialized_fill(tmp.begin(), tmp.begin() + count, value);
    tmp.set_size(count);
    m_storage.swap(tmp, false);
    // m_storage.reset();
    // if (count == 0)
    // {
    //     return;
    // }
    // auto lambda = [count, value](pointer dest) mutable -> void {
    //     while (count--)
    //     {
    //         pw::construct_at(dest++, value);
    //     }
    // };
    // m_storage.reserve(count, lambda);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(pw::initializer_list<value_type> init_list)
{
    Storage tmp { allocator_type(), init_list.size() };
    pw::uninitialized_copy(init_list.begin(), init_list.end(), tmp.begin());
    tmp.set_size(init_list.size());
    m_storage.swap(tmp, false);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::allocator_type
vector<Type, Allocator>::get_allocator() const
{
    return m_storage.get_allocator();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::at(size_type position)
{
    if (position >= size())
    {
        throw std::out_of_range("vector::at");
    }
    return *(m_storage.begin() + position);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::at(size_type position) const
{
    if (position >= size())
    {
        throw std::out_of_range("vector::at");
    }
    return *(m_storage.begin() + position);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::operator[](size_type position)
{
    return *(m_storage.begin() + position);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::operator[](size_type position) const
{
    return *(m_storage.begin() + position);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::front()
{
    return *m_storage.begin();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::front() const
{
    return *m_storage.begin();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::back()
{
    return *(m_storage.end() - 1);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::back() const
{
    return *(m_storage.end() - 1);
}

template<class Type, class Allocator>
constexpr Type*
vector<Type, Allocator>::data() noexcept
{
    if (empty())
        return nullptr;
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr Type const*
vector<Type, Allocator>::data() const noexcept
{
    if (empty())
        return nullptr;
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::begin() noexcept
{
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::begin() const noexcept
{
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::end() noexcept
{
    return m_storage.end();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::end() const noexcept
{
    return m_storage.end();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cbegin() const noexcept
{
    return m_storage.begin();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cend() const noexcept
{
    return m_storage.end();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(cbegin());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(cbegin());
}

template<class Type, class Allocator>
constexpr bool
vector<Type, Allocator>::empty() const noexcept
{
    return m_storage.empty();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::size() const noexcept
{
    return m_storage.size();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
// ReSharper disable once CppMemberFunctionMayBeStatic
vector<Type, Allocator>::max_size() const noexcept
{
    return (static_cast<size_type>(1) << (sizeof(size_type) * 8 - 4)) / sizeof(value_type);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::capacity() const noexcept
{
    return m_storage.allocated();
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::shrink_to_fit()
{
    if (m_storage.allocated() == m_storage.size())
    {
        return;
    }
    Storage tmp(m_storage.get_allocator(), m_storage.size());
    pw::uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
    tmp.set_size(m_storage.size());
    m_storage.swap(tmp, false);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::reserve(size_type count)
{
    if (count <= m_storage.allocated())
        return;
    Storage tmp(m_storage.get_allocator(), count);
    pw::uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
    tmp.set_size(m_storage.size());
    m_storage.swap(tmp, false);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::clear() noexcept
{
    // Calling clear() does not affect the result of capacity().
    pw::destroy(m_storage.begin(), m_storage.end());
    m_storage.set_size(0);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(const_reference value)
{
    constexpr size_type count = 1;
    size_type const     total = m_storage.size() + count;

    if (total <= m_storage.allocated())
    {
        pw::allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*m_storage.end()), value);
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), m_storage.calc_size());

        pw::uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
        pw::allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*(tmp.begin() + total - count)), value);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(value_type&& value)
{
    constexpr size_type count = 1;
    size_type const     total = m_storage.size() + count;

    if (total <= m_storage.allocated())
    {
        pw::construct_at(pw::addressof(*m_storage.end()), pw::move(value));
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), m_storage.calc_size());

        pw::uninitialized_move(m_storage.begin(), m_storage.end(), tmp.begin());
        pw::allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*(tmp.begin() + total - 1)), pw::move(value));
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type total)
{
    if (total == 0)
    {
        clear();
    }
    else if (total <= size())
    {
        pw::destroy(m_storage.begin() + total, m_storage.end());
    }
    else if (total <= m_storage.allocated())
    {
        pw::uninitialized_default_construct(m_storage.end(), m_storage.end() + total - size());
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), total);

        pw::uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
        pw::uninitialized_default_construct(tmp.begin() + m_storage.size(), tmp.begin() + total);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type total, const_reference value)
{
    if (total == 0)
    {
        clear();
    }
    else if (total <= size())
    {
        pw::destroy(m_storage.begin() + total, m_storage.end());
    }
    else if (total <= m_storage.allocated())
    {
        pw::uninitialized_fill(m_storage.end(), m_storage.end() + total - size(), value);
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), total);

        pw::uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
        pw::uninitialized_fill(tmp.begin() + m_storage.size(), tmp.begin() + total, value);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::erase(const_iterator position)
{
    return erase(position, position + 1);
}

/**
 * Removes the elements in the range [begin, end).
 *
 * @param begin Iterator to the first element to remove
 * @param end   Iterator to one past the last element to remove
 *
 * @return Iterator following the last removed element.
 *         - If end == end() prior to removal, then the updated end() iterator is returned.
 *         - If [begin, end) is an empty range, then end is returned.
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::erase(const_iterator begin, const_iterator end)
{
    size_type const offset = pw::distance(cbegin(), begin);
    iterator        dest   = m_storage.begin() + offset;
    iterator        last   = m_storage.begin() + pw::distance(cbegin(), end);

    if (begin == end)
    {
        return last;
    }
    pw::move(last, m_storage.end(), dest);
    pw::destroy(last, m_storage.end());
    m_storage.set_size(m_storage.size() - pw::distance(begin, end));
    return dest;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::pop_back()
{
    (void)erase(end() - 1);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, const_reference value)
{
    return insert(position, static_cast<size_type>(1), value);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, value_type&& value)
{
    constexpr size_type count  = 1;
    size_type const     offset = pw::distance(cbegin(), position);
    size_type const     total  = size() + count;

    if (total <= m_storage.allocated())
    {
        if (position == cend())
        {
            pw::uninitialized_fill(m_storage.end(), m_storage.end() + count, pw::move(value));
        }
        else
        {
            pw::uninitialized_move(m_storage.end() - count, m_storage.end(), m_storage.end());
            pw::move_backward(m_storage.begin() + offset, m_storage.end() - count, m_storage.end());
            pw::fill_n(m_storage.begin() + offset, count, value);
        }
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), m_storage.calc_size());

        pw::uninitialized_move(m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + count);
        pw::allocator_traits<Allocator>::construct(
            tmp.allocator(), pw::addressof(*(tmp.begin() + offset)), value);
        pw::uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, size_type count, const_reference value)
{
    size_type const offset = pw::distance(cbegin(), position);
    size_type const total  = size() + count;

    if (total < m_storage.allocated())
    {
        if (position == cend())
        {
            pw::uninitialized_fill(m_storage.end(), m_storage.end() + count, value);
        }
        else
        {
            pw::uninitialized_move(m_storage.end() - count, m_storage.end(), m_storage.end());
            pw::move_backward(m_storage.begin() + offset, m_storage.end() - count, m_storage.end());
            // TODO: Can this fill to uninitialized memory?
            pw::fill_n(m_storage.begin() + offset, count, value);
        }
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), total);

        pw::uninitialized_move(m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + count);
        pw::uninitialized_fill(tmp.begin() + offset, tmp.begin() + offset + count, value);
        pw::uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, Iterator first, Iterator last)
{
    size_type offset = pw::distance(cbegin(), position);
    iterator  dest   = m_storage.begin() + offset;

    while (first != last)
    {
        dest = insert(dest, *first) + 1;
        ++first;
    }
    return m_storage.begin() + offset;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, pw::initializer_list<value_type> init_list)
{
    size_type       offset = pw::distance(cbegin(), position);
    size_type const total  = size() + init_list.size();

    if (total <= m_storage.allocated())
    {
        pw::uninitialized_move(m_storage.end() - init_list.size(), m_storage.end(), m_storage.end());
        pw::move_backward(m_storage.begin() + offset, m_storage.end() - init_list.size(), m_storage.end());
        pw::move(init_list.begin(), init_list.end(), m_storage.begin() + offset);
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), m_storage.size() + init_list.size());

        pw::uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        pw::uninitialized_move(init_list.begin(), init_list.end(), tmp.begin() + offset);
        if (!m_storage.empty())
        {
            pw::uninitialized_move(
                m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + init_list.size());
        }
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

template<class Type, class Allocator>
template<class... Args>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::emplace_back(Args&&... args)
{
    size_type const total = size() + 1;
    if (total <= capacity())
    {
        allocator_traits<Allocator>::construct(m_storage.allocator(),
                                               pw::addressof(*(m_storage.begin() + total - 1)),
                                               pw::forward<Args>(args)...);
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), m_storage.calc_size());

        pw::uninitialized_move(m_storage.begin(), m_storage.end(), tmp.begin());
        allocator_traits<Allocator>::construct(
            tmp.allocator(), pw::addressof(*(tmp.begin() + total - 1)), pw::forward<Args>(args)...);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return back();
}

template<class Type, class Allocator>
template<class... Args>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::emplace(const_iterator position, Args&&... args)
{
    size_type const     total  = size() + 1;
    constexpr size_type count  = 1;
    size_type const     offset = pw::distance(cbegin(), position);
    iterator            where  = m_storage.begin() + offset;

    if (total <= capacity())
    {
        pw::uninitialized_move(m_storage.end() - count, m_storage.end(), m_storage.end());
        pw::move_backward(m_storage.begin() + offset, m_storage.end(), m_storage.end() - count);
        allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*(m_storage.begin() + offset)), pw::forward<Args>(args)...);
    }
    else
    {
        Storage tmp(m_storage.get_allocator(), m_storage.calc_size());

        pw::uninitialized_move(m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + count);
        pw::allocator_traits<Allocator>::construct(
            tmp.allocator(), pw::addressof(*(tmp.begin() + offset)), pw::forward<Args>(args)...);
        pw::uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

template<class Type, class Allocator>
constexpr void
swap(vector<Type, Allocator>& op1, vector<Type, Allocator>& op2) noexcept(noexcept(op1.swap(op2)))
{
    op1.swap(op2);
}

template<class Type, class Allocator>
constexpr bool
operator==(const pw::vector<Type, Allocator>& op1, const pw::vector<Type, Allocator>& op2)
{
    if (op1.size() != op2.size())
    {
        return false;
    }
    return op1 <=> op2 == 0;
}

template<class Type, class Allocator>
constexpr auto
operator<=>(const pw::vector<Type, Allocator>& op1, const pw::vector<Type, Allocator>& op2)
    -> decltype(op1[0] <=> op2[0])
{
    for (size_t i = 0; i < pw::min(op1.size(), op2.size()); ++i)
    {
        auto cmp = op1[i] <=> op2[i];
        if (cmp != 0)
        {
            return cmp;
        }
    }
    if (op1.size() < op2.size())
    {
        return std::strong_ordering::less;
    }
    else if (op1.size() > op2.size())
    {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_VECTOR_DEFN_H */
