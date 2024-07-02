#pragma once

template<typename T>
class Iter
{
public:
	Iter(T* ptr) noexcept :mptr{ ptr } {}

	Iter& operator++() noexcept
	{
		++mptr;
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
		--mptr;
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
		return *mptr;
	}

	friend inline bool operator==(const Iter& lhs, const Iter& rhs) noexcept
	{
		return lhs.mptr == rhs.mptr;
	}

	friend inline bool operator!=(const Iter& lhs, const Iter& rhs) noexcept
	{
		return !(lhs == rhs);
	}

private:
	T* mptr;
};
