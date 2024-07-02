#pragma once

#include "Iter.tpp"

template <typename T, unsigned N>
class StaticVector
{
public:
	StaticVector() noexcept {}

	T at(unsigned n) const noexcept
	{
		return mData[n];
	}

	T& at(unsigned n) noexcept
	{
		return mData[n];
	}

	T operator[](unsigned n) const noexcept
	{
		return mData[n];
	}

	T& operator[](unsigned n) noexcept
	{
		return mData[n];
	}

	T front() const noexcept
	{
		return mData[0];
	}

	T& front() noexcept
	{
		return mData[0];
	}

	T back() const noexcept
	{
		return mData[mSize - 1];
	}

	T& back() noexcept
	{
		return mData[mSize - 1];
	}

	T* data() noexcept
	{
		return mData;
	}

	Iter<T> begin() noexcept
	{
		return Iter<T>{mData};
	}

	Iter<T> end() noexcept
	{
		return Iter<T>{mData + mSize};
	}

	bool empty() const noexcept
	{
		return mSize == 0;
	}

	unsigned size() const noexcept
	{
		return mSize;
	}

	void clear() noexcept
	{
		for (unsigned i{ 0 }; i < mSize; ++i)
		{
			mData[i] = 0;
		}
	}

private:
	T mData[N + 1]{ T() };
	unsigned mSize{ N };
};
