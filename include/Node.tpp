#pragma once

// TODO: remove standard library
#include <utility>

template<typename T>
class Node
{
public:
    constexpr Node() noexcept :
        mData { T {} }
    {
    }

    constexpr ~Node() noexcept = default;
    constexpr Node(const Node&) noexcept = default;
    constexpr Node(Node&&) noexcept = default;
    constexpr Node& operator=(const Node&) noexcept = default;
    constexpr Node& operator=(Node&&) noexcept = default;

    constexpr Node(const T& data) noexcept :
        mData { data }
    {
    }

    constexpr Node(T&& data) noexcept :
        mData { std::move(data) }
    {
    }

    constexpr T data() const noexcept
    {
        return mData;
    }

    constexpr T& data() noexcept
    {
        return mData;
    }

    constexpr Node* next() const noexcept
    {
        return mNext;
    }

    constexpr Node*& next() noexcept
    {
        return mNext;
    }

    constexpr Node* prev() const noexcept
    {
        return mPrev;
    }

    constexpr Node*& prev() noexcept
    {
        return mPrev;
    }
private:
    T mData;
    Node* mNext { nullptr };
    Node* mPrev { nullptr };
};
