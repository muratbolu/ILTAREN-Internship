#pragma once

#include "IIter.tpp"

template<typename T>
class ContIter : IIter<T>
{
public:
	constexpr ContIter(T* ptr) noexcept :mPtr{ ptr } {}

	constexpr ContIter& operator++() noexcept
	{
		++mPtr;
		return *this;
	}

	constexpr ContIter operator++(int) noexcept
	{
		ContIter old = *this;
		operator++();
		return old;
	}

	constexpr ContIter& operator--() noexcept
	{
		--mPtr;
		return *this;
	}

	constexpr ContIter operator--(int) noexcept
	{
		ContIter old = *this;
		operator--();
		return old;
	}

	constexpr T& operator*() const noexcept
	{
		return *mPtr;
	}

	// operator!= is automatically generated
	constexpr friend inline bool operator==(const ContIter& lhs, const ContIter& rhs) noexcept
	{
		return lhs.mPtr == rhs.mPtr;
	}

	constexpr inline bool operator==(const T* rhs) noexcept
	{
		return mPtr == rhs;
	}

	// operators <, <=, >, >=  are automatically generated
	constexpr friend inline auto operator<=>(const ContIter& lhs, const ContIter& rhs) noexcept
	{
		return lhs.mPtr <=> rhs.mPtr;
	}

	constexpr inline auto operator<=>(const T* rhs) noexcept
	{
		return mPtr <=> rhs;
	}

private:
	T* mPtr;
};
