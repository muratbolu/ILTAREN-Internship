#pragma once

#include <cassert>
#include <compare>
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
        mHour = parseTimeDigits(arg[0], arg[1]);
        mMinute = parseTimeDigits(arg[3], arg[4]);
        // NOLINTEND
        assert(mHour < 24);
        assert(mMinute < 60);
    }

    [[nodiscard]] constexpr unsigned getHour() const noexcept
    {
        return mHour;
    }

    [[nodiscard]] constexpr unsigned getMinute() const noexcept
    {
        return mMinute;
    }

    // operator!= is automatically generated
    constexpr friend inline bool operator==(const Time& lhs, const Time& rhs) noexcept = default;

    // operators <, <=, >, >=  are automatically generated
    constexpr friend inline auto operator<=>(const Time& lhs, const Time& rhs) noexcept
    {
        std::strong_ordering s { lhs.mHour <=> rhs.mHour };
        if (s == 0)
        {
            return lhs.mMinute <=> rhs.mMinute;
        }
        return s;
    }
private:
    uint8_t mHour { 0 }, mMinute { 0 };

    constexpr static uint8_t parseTimeDigits(char c1, char c2) noexcept
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

    // operator!= is automatically generated
    constexpr inline bool operator==(const Duration&) const noexcept = default;
    // operators <, <=, >, >=  are automatically generated
    constexpr inline auto operator<=>(const Duration&) const noexcept = default;
private:
    unsigned mDur { 0 };
};
}   // namespace chr
