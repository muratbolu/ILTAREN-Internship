#pragma once

template<typename T>
class IObjectPool
{
public:
    constexpr IObjectPool() noexcept = default;
    virtual constexpr ~IObjectPool() noexcept = default;
    constexpr IObjectPool(const IObjectPool&) noexcept = default;
    constexpr IObjectPool(IObjectPool&&) noexcept = default;
    constexpr IObjectPool& operator=(const IObjectPool&) noexcept = default;
    constexpr IObjectPool& operator=(IObjectPool&&) noexcept = default;

    virtual constexpr T* allocate() noexcept = 0;
    virtual constexpr bool deallocate(T* ptr) noexcept = 0;
};
