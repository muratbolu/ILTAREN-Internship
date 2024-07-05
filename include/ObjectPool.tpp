#pragma once

#include "IObjectPool.tpp"
#include "StaticVector.tpp"

template<typename T, unsigned N>
class ObjectPool : public IObjectPool<T>
{
public:
	constexpr ObjectPool() noexcept
	{
		for (bool& i : mVacant)
		{
			i = true;
		}
	}
	constexpr ~ObjectPool() noexcept override = default;
	constexpr ObjectPool(const ObjectPool&) noexcept = default;
	constexpr ObjectPool(ObjectPool&&) noexcept = default;
	constexpr ObjectPool& operator=(const ObjectPool&) noexcept = default;
	constexpr ObjectPool& operator=(ObjectPool&&) noexcept = default;

	constexpr T* allocate() noexcept override
	{
		for (unsigned i{ 0 }; i < N; ++i)
		{
			if (mVacant[i] == true)
			{
				mVacant[i] = false;
				// Clean the memory area
				mPool[i] = T{};
				return &mPool[i];
			}
		}
		return nullptr;
	}

	constexpr bool deallocate(T* ptr) noexcept override
	{
		if (ptr != nullptr
			&& mPool.begin() <= ptr
			&& ptr < mPool.end()
			&& mVacant[ptr - mPool.data()] == false)
		{
			mVacant[ptr - mPool.data()] = true;
			return true;
		}
		return false;
	}

private:
	StaticVector<T, N> mPool;
	StaticVector<bool, N> mVacant;
};
