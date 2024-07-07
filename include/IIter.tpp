#pragma once

template<typename T>
class IIter
{
public:
    constexpr IIter() noexcept = default;
    virtual constexpr inline ~IIter() noexcept = default;
    constexpr IIter(const IIter&) noexcept = default;
    constexpr IIter(IIter&&) noexcept = default;
    constexpr IIter& operator=(const IIter&) noexcept = default;
    constexpr IIter& operator=(IIter&&) noexcept = default;

    virtual constexpr IIter& next() noexcept = 0;
    virtual constexpr IIter& prev() noexcept = 0;
    virtual constexpr T& data() noexcept = 0;
    virtual constexpr T data() const noexcept = 0;
};
