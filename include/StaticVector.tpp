#pragma once

#include "ContIter.tpp"

template<typename T, unsigned N>
class StaticVector
{
public:
	constexpr StaticVector() noexcept {}

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

	constexpr T back() const noexcept
	{
		return mData[mSize - 1];
	}

	constexpr T& back() noexcept
	{
		return mData[mSize - 1];
	}

	constexpr T* data() noexcept
	{
		return mData;
	}

	constexpr ContIter<T> begin() noexcept
	{
		return ContIter<T>{mData};
	}

	constexpr ContIter<T> end() noexcept
	{
		return ContIter<T>{mData + mSize};
	}

	constexpr bool empty() const noexcept
	{
		return mSize == 0;
	}

	constexpr unsigned size() const noexcept
	{
		return mSize;
	}

	constexpr void clear() noexcept
	{
		for (unsigned i{ 0 }; i < mSize; ++i)
		{
			mData[i] = 0;
		}
	}

private:
	T mData[N + 1]{ T() };
	const unsigned mSize{ N };
};
