#pragma once

#include "StaticVector.tpp"

template<typename T, unsigned N>
class ObjectPool
{
public:
	ObjectPool() noexcept {}
	T* allocate(unsigned n) noexcept
	{
		;
	}
	void deallocate(T* ptr) noexcept
	{
		;
	}

private:
	StaticVector<T, N> pool;
	unsigned head{ 0 }, tail{ 0 };
};
