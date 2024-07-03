#pragma once

template<typename T>
class IIter
{
public:
	virtual constexpr IIter& next() noexcept = 0;
	virtual constexpr IIter& prev() noexcept = 0;
	virtual constexpr T& data() const noexcept = 0;
};
