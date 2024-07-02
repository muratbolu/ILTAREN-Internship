#pragma once

#include "iter.tpp"

template <typename T, unsigned N>
class static_vector
{
public:
	static_vector() noexcept {}

	T at(unsigned n) const noexcept
	{
		return mdata[n];
	}

	T& at(unsigned n) noexcept
	{
		return mdata[n];
	}

	T operator[](unsigned n) const noexcept
	{
		return mdata[n];
	}

	T& operator[](unsigned n) noexcept
	{
		return mdata[n];
	}

	T front() const noexcept
	{
		return mdata[0];
	}

	T& front() noexcept
	{
		return mdata[0];
	}

	T back() const noexcept
	{
		return mdata[msize - 1];
	}

	T& back() noexcept
	{
		return mdata[msize - 1];
	}

	T* data() noexcept
	{
		return mdata;
	}

	iter<T> begin() noexcept
	{
		return iter<T>{mdata};
	}

	iter<T> end() noexcept
	{
		return iter<T>{mdata + msize};
	}

	bool empty() const noexcept
	{
		return msize == 0;
	}

	unsigned size() const noexcept
	{
		return msize;
	}

	void clear() noexcept
	{
		for (unsigned i{ 0 }; i < msize; ++i)
		{
			mdata[i] = 0;
		}
	}

private:
	T mdata[N + 1]{ T() };
	unsigned msize{ N };
};
