#pragma once

#include <cassert>
#include <cstdint>

namespace chr
{
class Time
{
public:
    constexpr Time() noexcept = default;

    constexpr explicit Time(const char arg[5]) noexcept
    {
        // NOLINTBEGIN
        assert((arg[2] == ':') || (arg[2] == '.'));
        mHour = parseTwoDigits(arg[0], arg[1]);
        mMinute = parseTwoDigits(arg[3], arg[4]);
        // NOLINTEND
    }

    [[nodiscard]] constexpr unsigned getHour() const noexcept
    {
        return mHour;
    }

    [[nodiscard]] constexpr unsigned getMinute() const noexcept
    {
        return mMinute;
    }
private:
    uint8_t mHour { 0 }, mMinute { 0 };

    constexpr static uint8_t parseTwoDigits(char c1, char c2) noexcept
    {
        return static_cast<uint8_t>(10 * (c1 - '0') + (c2 - '0'));
    }
};

class Duration
{
public:
    constexpr Duration() noexcept = default;

    constexpr Duration(unsigned dur) noexcept :
        mDur { dur }
    {
    }

    [[nodiscard]] constexpr unsigned getDuration() const noexcept
    {
        return mDur;
    }
private:
    unsigned mDur { 0 };
};
}   // namespace chr
