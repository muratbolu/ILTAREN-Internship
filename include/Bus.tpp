#pragma once

#include <cassert>
#include <utility>
#include <variant>

class Bus
{
public:
    using ConstantPeriod = unsigned;
    using AlternatingPeriod = std::pair<unsigned, unsigned>;
    using Period = std::variant<ConstantPeriod, AlternatingPeriod>;

    constexpr Bus(unsigned i = 0) noexcept :
        mPeriod { i }
    {
        assert(std::holds_alternative<ConstantPeriod>(mPeriod));
    }

    constexpr Bus(unsigned i, unsigned j) noexcept :
        mPeriod { std::make_pair(i, j) }
    {
        assert(i != 0);
        assert(j != 0);
        assert(i != j);
        assert((std::holds_alternative<AlternatingPeriod>(mPeriod)));
    }

    constexpr Bus(const Bus&) noexcept = default;
    constexpr Bus(Bus&&) noexcept = default;
    constexpr Bus& operator=(const Bus&) noexcept = default;
    constexpr Bus& operator=(Bus&&) noexcept = default;

    [[nodiscard]] constexpr unsigned getFirst() const noexcept
    {
        if (const auto* p { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *p;
        }
        else if (const auto* p { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return p->first;
        }
        return 0;
    }

    [[nodiscard]] constexpr unsigned getSecond() const noexcept
    {
        if (const auto* p { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *p;
        }
        else if (const auto* p { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return p->second;
        }
        return 0;
    }

    [[nodiscard]] constexpr unsigned sum() const noexcept
    {
        if (const auto* p { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *p + *p;
        }
        else if (const auto* p { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return p->first + p->second;
        }
        return 0;
    }

    [[nodiscard]] constexpr bool isAlternating() const noexcept
    {
        return std::holds_alternative<std::pair<unsigned, unsigned>>(mPeriod);
    }

    [[nodiscard]] constexpr bool isValid() const noexcept
    {
        if (const auto* p { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *p != 0;
        }
        return true;
    }

    constexpr inline Bus& operator*=(unsigned i) noexcept
    {
        if (auto* p { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            *p *= i;
        }
        else if (auto* p { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            p->first *= i;
            p->second *= i;
        }
        return *this;
    }

    constexpr friend inline Bus operator*(Bus t, unsigned i) noexcept
    {
        t *= i;
        return t;
    }

    // operator!= is automatically generated
    constexpr friend inline bool operator==(const Bus&, const Bus&) noexcept = default;
private:
    Period mPeriod;
};
