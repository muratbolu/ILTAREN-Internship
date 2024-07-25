#pragma once

#include "iter/ContIter.tpp"

template<typename T, unsigned N>
class StaticVector
{
public:
    constexpr StaticVector() noexcept = default;
    virtual constexpr ~StaticVector() noexcept = default;
    constexpr StaticVector(const StaticVector&) noexcept = default;
    constexpr StaticVector(StaticVector&&) noexcept = default;
    constexpr StaticVector& operator=(const StaticVector&) noexcept = default;
    constexpr StaticVector& operator=(StaticVector&&) noexcept = default;

    constexpr T at(unsigned n) const noexcept
    {
        return mData[n];
    }

    constexpr T& at(unsigned n) noexcept
    {
        return mData[n];
    }

    constexpr T operator[](unsigned n) const noexcept
    {
        return mData[n];
    }

    constexpr T& operator[](unsigned n) noexcept
    {
        return mData[n];
    }

    constexpr T front() const noexcept
    {
        return mData[0];
    }

    constexpr T& front() noexcept
    {
        return mData[0];
    }

    virtual constexpr T back() const noexcept
    {
        return mData[mSize - 1];
    }

    virtual constexpr T& back() noexcept
    {
        return mData[mSize - 1];
    }

    constexpr T* data() noexcept
    {
        return mData;
    }

    constexpr ContIter<T> begin() noexcept
    {
        return ContIter<T> { mData };
    }

    virtual constexpr ContIter<T> end() noexcept
    {
        return ContIter<T> { mData + mSize };
    }

    [[nodiscard]] virtual constexpr bool empty() const noexcept
    {
        return mSize == 0;
    }

    [[nodiscard]] virtual constexpr unsigned size() const noexcept
    {
        return mSize;
    }

    constexpr void fill(const T& obj) noexcept
    {
        for (unsigned i { 0 }; i < mSize; ++i)
        {
            mData[i] = obj;
        }
    }

    virtual constexpr bool contains(const T& obj) const noexcept
    {
        for (unsigned i { 0 }; i < mSize; ++i)
        {
            if (obj == mData[i])
            {
                return true;
            }
        }
        return false;
    }

    virtual constexpr unsigned count(const T& obj) const noexcept
    {
        unsigned num { 0 };
        for (unsigned i { 0 }; i < mSize; ++i)
        {
            if (obj == mData[i])
            {
                ++num;
            }
        }
        return num;
    }

    // operator!= is automatically generated
    constexpr friend bool operator==(const StaticVector& lhs, const StaticVector& rhs) noexcept
    {
        for (unsigned i { 0 }; i < N; ++i)
        {
            if (lhs[i] != rhs[i])
            {
                return false;
            }
        }
        return true;
    }
private:
    T mData[N];
    unsigned mSize { N };
};
