#pragma once

template<typename T>
class Iter
{
public:
	Iter(T* ptr) noexcept :mPtr{ ptr } {}

	Iter& operator++() noexcept
	{
		++mPtr;
		return *this;
	}

	Iter operator++(int) noexcept
	{
		Iter old = *this;
		operator++();
		return old;
	}

	Iter& operator--() noexcept
	{
		--mPtr;
		return *this;
	}

	Iter operator--(int) noexcept
	{
		Iter old = *this;
		operator--();
		return old;
	}

	T& operator*() const noexcept
	{
		return *mPtr;
	}

	// operator!= is automatically generated
	friend inline bool operator==(const Iter& lhs, const Iter& rhs) noexcept
	{
		return lhs.mPtr == rhs.mPtr;
	}

	inline bool operator==(const T* rhs) noexcept
	{
		return mPtr == rhs;
	}

	// operators <, <=, >, >=  are automatically generated
	friend inline auto operator<=>(const Iter& lhs, const Iter& rhs) noexcept
	{
		return lhs.mPtr <=> rhs.mPtr;
	}

	inline auto operator<=>(const T* rhs) noexcept
	{
		return mPtr <=> rhs;
	}

private:
	T* mPtr;
};
