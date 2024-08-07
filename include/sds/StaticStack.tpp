#pragma once

#include "iter/ContIter.tpp"
#include "sds/StaticVector.tpp"

template<typename T, unsigned N>
class StaticStack : public StaticVector<T, N>
{
public:
    constexpr StaticStack() noexcept = default;
    constexpr ~StaticStack() noexcept override = default;
    constexpr StaticStack(const StaticStack&) noexcept = default;
    constexpr StaticStack(StaticStack&&) noexcept = default;
    constexpr StaticStack& operator=(const StaticStack&) noexcept = default;
    constexpr StaticStack& operator=(StaticStack&&) noexcept = default;

    constexpr T back() const noexcept override
    {
        return this->at(mCurrIndex - 1);
    }

    constexpr T& back() noexcept override
    {
        return this->at(mCurrIndex - 1);
    }

    constexpr T top() const noexcept
    {
        return back();
    }

    constexpr T& top() noexcept
    {
        return back();
    }

    constexpr ContIter<T> end() noexcept override
    {
        return ContIter<T> { this->data() + mCurrIndex };
    }

    [[nodiscard]] constexpr bool empty() const noexcept override
    {
        return mCurrIndex == 0;
    }

    [[nodiscard]] constexpr unsigned size() const noexcept override
    {
        return mCurrIndex;
    }

    constexpr void clear() noexcept
    {
        mCurrIndex = 0;
    }

    constexpr bool contains(const T& obj) const noexcept override
    {
        for (unsigned i { 0 }; i < mCurrIndex; ++i)
        {
            if (obj == this->at(i))
            {
                return true;
            }
        }
        return false;
    }

    constexpr unsigned count(const T& obj) const noexcept override
    {
        unsigned num { 0 };
        for (unsigned i { 0 }; i < mCurrIndex; ++i)
        {
            if (obj == this->at(i))
            {
                ++num;
            }
        }
        return num;
    }

    // operator!= is automatically generated
    template<unsigned U>
    constexpr friend bool operator==(const StaticStack<T, N>& lhs, const StaticStack<T, U>& rhs) noexcept
    {
        if (lhs.mCurrIndex != rhs.mCurrIndex)
        {
            return false;
        }
        for (unsigned i { 0 }; i < lhs.mCurrIndex; ++i)
        {
            if (lhs[i] != rhs[i])
            {
                return false;
            }
        }
        return true;
    }

    constexpr void pushBack(const T& obj) noexcept
    {
        mMaxIndex++;
        this->at(mCurrIndex++) = obj;
    }

    constexpr T popBack() noexcept
    {
        return this->at(--mCurrIndex);
    }

    constexpr void sort() noexcept
    {
        if (mCurrIndex <= 1)
        {
            return;
        }
        for (unsigned i { 0 }; i < mCurrIndex; ++i)
        {
            for (unsigned j { i }; j > 0 && this->at(j - 1) > this->at(j); --j)
            {
                T temp { this->at(j - 1) };
                this->at(j - 1) = this->at(j);
                this->at(j) = temp;
            }
        }
    }

    unsigned mMaxIndex { 0 };
private:
    unsigned mCurrIndex { 0 };
};
