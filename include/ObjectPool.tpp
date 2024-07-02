#pragma once

#include "StaticVector.tpp"

template<typename T, unsigned N>
class ObjectPool
{
public:
	ObjectPool() noexcept
	{
		for (bool& i : vacancy)
		{
			i = true;
		}
	}
	T* allocate() noexcept
	{
		for (unsigned i{ 0 }; i < N; ++i)
		{
			if (vacancy[i] == true)
			{
				vacancy[i] = false;
				return &pool[i];
			}
		}
		return nullptr;
	}
	bool deallocate(T* ptr) noexcept
	{
		if (pool.data() <= ptr
			&& ptr < (pool.data() + N)
			&& vacancy[ptr - pool.data()] == false)
		{
			vacancy[ptr - pool.data()] = true;
			return true;
		}
		return false;
	}

private:
	StaticVector<T, N> pool;
	StaticVector<bool, N> vacancy;
};
