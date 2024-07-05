#pragma once

template<typename T>
class IIter
{
public:
	virtual constexpr inline ~IIter() noexcept {};
	virtual constexpr IIter& next() noexcept = 0;
	virtual constexpr IIter& prev() noexcept = 0;
	virtual constexpr T& data() noexcept = 0;
	virtual constexpr T data() const noexcept = 0;
};
