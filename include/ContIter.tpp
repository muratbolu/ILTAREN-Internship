#pragma once

#include "IIter.tpp"

template<typename T>
class ContIter : public IIter<T>
{
public:
	constexpr ContIter(T* ptr) noexcept :mPtr{ ptr } {}
	constexpr ~ContIter() noexcept override = default;
	constexpr ContIter(const ContIter&) noexcept = default;
	constexpr ContIter(ContIter&&) noexcept = default;
	constexpr ContIter& operator=(const ContIter&) noexcept = default;
	constexpr ContIter& operator=(ContIter&&) noexcept = default;

	constexpr ContIter& next() noexcept override
	{
		++mPtr;
		return *this;
	}

	constexpr ContIter& operator++() noexcept
	{
		return next();
	}

	constexpr ContIter operator++(int) noexcept
	{
		ContIter old = *this;
		operator++();
		return old;
	}

	constexpr ContIter& prev() noexcept override
	{
		--mPtr;
		return *this;
	}

	constexpr ContIter& operator--() noexcept
	{
		return prev();
	}

	constexpr ContIter operator--(int) noexcept
	{
		ContIter old = *this;
		operator--();
		return old;
	}

	constexpr T& data() noexcept override
	{
		return *mPtr;
	}

	constexpr T data() const noexcept override
	{
		return *mPtr;
	}

	constexpr T& operator*() noexcept
	{
		return data();
	}

	constexpr T operator*() const noexcept
	{
		return data();
	}

	// operator!= is automatically generated
	constexpr friend inline static bool operator==(const ContIter& lhs, const ContIter& rhs) noexcept
	{
		return lhs.mPtr == rhs.mPtr;
	}

	constexpr inline bool operator==(const T* rhs) const noexcept
	{
		return mPtr == rhs;
	}

	// operators <, <=, >, >=  are automatically generated
	constexpr friend inline static auto operator<=>(const ContIter& lhs, const ContIter& rhs) noexcept
	{
		return lhs.mPtr <=> rhs.mPtr;
	}

	constexpr inline auto operator<=>(const T* rhs) const noexcept
	{
		return mPtr <=> rhs;
	}

private:
	T* mPtr;
};
