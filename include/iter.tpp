#pragma once

template<typename T>
class Iter
{
public:
	Iter(T* ptr) noexcept :ptr{ ptr } {}

	Iter& operator++() noexcept
	{
		++ptr;
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
		--ptr;
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
		return *ptr;
	}

	friend inline bool operator==(const Iter& lhs, const Iter& rhs) noexcept
	{
		return lhs.ptr == rhs.ptr;
	}

	friend inline bool operator!=(const Iter& lhs, const Iter& rhs) noexcept
	{
		return !(lhs == rhs);
	}

private:
	T* ptr;
};
