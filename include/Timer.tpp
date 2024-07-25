#pragma once

#include <cassert>
#include <compare>
#include <cstdint>

namespace timer
{
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

    constexpr Duration& operator+=(const Duration& dur) noexcept
    {
        add(dur);
        return *this;
    }

    constexpr friend Duration operator+(Duration lhs, const Duration& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    // operator!= is automatically generated
    constexpr bool operator==(const Duration&) const noexcept = default;
    // operators <, <=, >, >=  are automatically generated
    constexpr auto operator<=>(const Duration&) const noexcept = default;
private:
    unsigned mDur { 0 };

    constexpr void add(const Duration& dur) noexcept
    {
        mDur += dur.mDur;
    }
};

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

    constexpr Time& operator+=(const Duration& dur) noexcept
    {
        add(dur);
        return *this;
    }

    constexpr friend Time operator+(Time t, const Duration& dur) noexcept
    {
        t += dur;
        return t;
    }

    constexpr friend Duration operator-(const Time& lhs, const Time& rhs) noexcept
    {
        return { 60 * (lhs.mHour - rhs.mHour) + lhs.mMinute - rhs.mMinute };
    }

    // operator!= is automatically generated
    constexpr friend bool operator==(const Time& lhs, const Time& rhs) noexcept = default;

    // operators <, <=, >, >=  are automatically generated
    constexpr friend auto operator<=>(const Time& lhs, const Time& rhs) noexcept
    {
        if (lhs.overflow)
        {
            return std::strong_ordering::greater;
        }
        std::strong_ordering s { lhs.mHour <=> rhs.mHour };
        if (s == 0)
        {
            return lhs.mMinute <=> rhs.mMinute;
        }
        return s;
    }
private:
    unsigned mHour { 0 }, mMinute { 0 };
    bool overflow { false };

    constexpr static uint8_t parseTimeDigits(char c1, char c2) noexcept
    {
        return static_cast<uint8_t>(10 * (c1 - '0') + (c2 - '0'));
    }

    constexpr void add(const Duration& dur) noexcept
    {
        if (overflow)
        {
            return;
        }
        mMinute += dur.getDuration();
        if (mMinute > 59)
        {
            mHour += mMinute / 60;
            if (mHour > 23)
            {
                overflow = true;
            }
            mMinute %= 60;
        }
    }
};
}   // namespace timer
