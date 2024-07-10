#pragma once

#include "IObjectPool.tpp"
#include "StaticVector.tpp"

template<typename T, unsigned N>
class ObjectPool : public IObjectPool<T>
{
public:
    constexpr ObjectPool() noexcept
    {
        mOccupied.clear();
    }

    constexpr ~ObjectPool() noexcept override = default;
    constexpr ObjectPool(const ObjectPool&) noexcept = default;
    constexpr ObjectPool(ObjectPool&&) noexcept = default;
    constexpr ObjectPool& operator=(const ObjectPool&) noexcept = default;
    constexpr ObjectPool& operator=(ObjectPool&&) noexcept = default;

    constexpr T* allocate() noexcept override
    {
        T* allocated { nullptr };
        ++maxAllocated;
        // TODO: implement a red-black tree for search
        for (unsigned i { 0 }; i < N; ++i)
        {
            if (!mOccupied[i])
            {
                mOccupied[i] = true;
                allocated = &mPool[i];
                break;
            }
        }
        return allocated;
    }

    constexpr bool deallocate(T* ptr) noexcept override
    {
        if (ptr != nullptr && mPool.begin() <= ptr && ptr < mPool.end() && !mOccupied[ptr - mPool.data()] == false)
        {
            mOccupied[ptr - mPool.data()] = false;
            return true;
        }
        return false;
    }

    unsigned maxAllocated { 0 };
private:
    StaticVector<T, N> mPool;
    StaticVector<bool, N> mOccupied;
};
