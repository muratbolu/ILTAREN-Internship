#pragma once

#include "StaticVector.tpp"

template<typename T, unsigned N>
class ObjectPool
{
public:
	ObjectPool() noexcept
	{
		for (bool& i : mvacancy)
		{
			i = true;
		}
	}
	T* allocate() noexcept
	{
		for (unsigned i{ 0 }; i < N; ++i)
		{
			if (mvacancy[i] == true)
			{
				mvacancy[i] = false;
				return &mpool[i];
			}
		}
		return nullptr;
	}
	bool deallocate(T* ptr) noexcept
	{
		if (mpool.data() <= ptr
			&& ptr < (mpool.data() + N)
			&& mvacancy[ptr - mpool.data()] == false)
		{
			mvacancy[ptr - mpool.data()] = true;
			return true;
		}
		return false;
	}

private:
	StaticVector<T, N> mpool;
	StaticVector<bool, N> mvacancy;
};
