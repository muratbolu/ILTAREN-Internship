#pragma once

#include "StaticVector.tpp"

template<typename T, unsigned N>
class ObjectPool
{
public:
	constexpr ObjectPool() noexcept
	{
		for (bool& i : mVacant)
		{
			i = true;
		}
	}
	constexpr T* allocate() noexcept
	{
		for (unsigned i{ 0 }; i < N; ++i)
		{
			if (mVacant[i] == true)
			{
				mVacant[i] = false;
				return &mPool[i];
			}
		}
		return nullptr;
	}
	constexpr bool deallocate(T* ptr) noexcept
	{
		if (mPool.begin() <= ptr
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
