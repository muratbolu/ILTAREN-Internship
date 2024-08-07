#pragma once

#include "iter/IIter.tpp"
#include "sds/Node.tpp"

#include <compare>

template<typename T>
class NodeIter : public IIter<T>
{
public:
    constexpr explicit NodeIter(Node<T>* ptr) noexcept :
        mPtr { ptr }
    {
    }

    constexpr ~NodeIter() noexcept override = default;
    constexpr NodeIter(const NodeIter&) noexcept = default;
    constexpr NodeIter(NodeIter&&) noexcept = default;
    constexpr NodeIter& operator=(const NodeIter&) noexcept = default;
    constexpr NodeIter& operator=(NodeIter&&) noexcept = default;

    constexpr NodeIter& next() noexcept override
    {
        mPtr = mPtr->next();
        return *this;
    }

    constexpr NodeIter& operator++() noexcept
    {
        return next();
    }

    constexpr NodeIter operator++(int) noexcept
    {
        NodeIter old = *this;
        operator++();
        return old;
    }

    constexpr NodeIter& prev() noexcept override
    {
        mPtr = mPtr->prev();
        return *this;
    }

    constexpr NodeIter& operator--() noexcept
    {
        return prev();
    }

    constexpr NodeIter operator--(int) noexcept
    {
        NodeIter old = *this;
        operator--();
        return old;
    }

    constexpr T& data() noexcept override
    {
        return mPtr->data();
    }

    constexpr T data() const noexcept override
    {
        return mPtr->data();
    }

    constexpr T& operator*() noexcept
    {
        return data();
    }

    constexpr T operator*() const noexcept
    {
        return data();
    }

    // operator!= is automatically generated
    constexpr friend bool operator==(const NodeIter& lhs, const NodeIter& rhs) noexcept
    {
        return lhs.mPtr == rhs.mPtr;
    }

    constexpr bool operator==(const T* rhs) const noexcept
    {
        return mPtr == rhs;
    }

    // operators <, <=, >, >=  are automatically generated
    constexpr friend auto operator<=>(const NodeIter& lhs, const NodeIter& rhs) noexcept
    {
        return lhs.mPtr <=> rhs.mPtr;
    }

    constexpr auto operator<=>(const T* rhs) const noexcept
    {
        return mPtr <=> rhs;
    }
private:
    Node<T>* mPtr;
};
