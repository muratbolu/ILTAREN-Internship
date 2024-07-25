#pragma once

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

    constexpr explicit Node(const T& data) noexcept :
        mData { data }
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

    // operator!= is automatically generated
    constexpr friend bool operator==(const Node& lhs, const Node& rhs) noexcept
    {
        return (lhs.mData == rhs.mData) && (lhs.mNext == rhs.mNext) && (lhs.mPrev == rhs.mPrev);
    }
private:
    T mData;
    Node* mNext { nullptr };
    Node* mPrev { nullptr };
};
