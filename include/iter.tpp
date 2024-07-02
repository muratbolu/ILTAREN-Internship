#pragma once

template<typename T>
class Iter
{
public:
	constexpr Iter(T* ptr) noexcept :mPtr{ ptr } {}

	constexpr Iter& operator++() noexcept
	{
		++mPtr;
		return *this;
	}

	constexpr Iter operator++(int) noexcept
	{
		Iter old = *this;
		operator++();
		return old;
	}

	constexpr Iter& operator--() noexcept
	{
		--mPtr;
		return *this;
	}

	constexpr Iter operator--(int) noexcept
	{
		Iter old = *this;
		operator--();
		return old;
	}

	constexpr T& operator*() const noexcept
	{
		return *mPtr;
	}

	// operator!= is automatically generated
	constexpr friend inline bool operator==(const Iter& lhs, const Iter& rhs) noexcept
	{
		return lhs.mPtr == rhs.mPtr;
	}

	constexpr inline bool operator==(const T* rhs) noexcept
	{
		return mPtr == rhs;
	}

	// operators <, <=, >, >=  are automatically generated
	constexpr friend inline auto operator<=>(const Iter& lhs, const Iter& rhs) noexcept
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
