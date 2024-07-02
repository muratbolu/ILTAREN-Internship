#pragma once

template<typename T>
class iter
{
public:
	iter(T* ptr) noexcept :ptr{ ptr } {}

	iter& operator++() noexcept
	{
		++ptr;
		return *this;
	}

	iter operator++(int) noexcept
	{
		iter old = *this;
		operator++();
		return old;
	}

	iter& operator--() noexcept
	{
		--ptr;
		return *this;
	}

	iter operator--(int) noexcept
	{
		iter old = *this;
		operator--();
		return old;
	}

	T& operator*() const noexcept
	{
		return *ptr;
	}

	friend inline bool operator==(const iter& lhs, const iter& rhs) noexcept
	{
		return lhs.ptr == rhs.ptr;
	}

	friend inline bool operator!=(const iter& lhs, const iter& rhs) noexcept
	{
		return !(lhs == rhs);
	}

private:
	T* ptr;
};
