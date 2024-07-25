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

    constexpr explicit Bus(unsigned i = 0) noexcept :
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

    constexpr ~Bus() noexcept = default;
    constexpr Bus(const Bus&) noexcept = default;
    constexpr Bus(Bus&&) noexcept = default;
    constexpr Bus& operator=(const Bus&) noexcept = default;
    constexpr Bus& operator=(Bus&&) noexcept = default;

    [[nodiscard]] constexpr unsigned getFirst() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *c;
        }
        if (const auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return a->first;
        }
        return 0;
    }

    [[nodiscard]] constexpr unsigned getSecond() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *c;
        }
        if (const auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return a->second;
        }
        return 0;
    }

    [[nodiscard]] constexpr unsigned sum() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *c + *c;
        }
        if (const auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return a->first + a->second;
        }
        return 0;
    }

    [[nodiscard]] constexpr bool isAlternating() const noexcept
    {
        return std::holds_alternative<std::pair<unsigned, unsigned>>(mPeriod);
    }

    [[nodiscard]] constexpr bool isValid() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *c != 0;
        }
        return true;
    }

    constexpr Bus& operator*=(unsigned i) noexcept
    {
        if (auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            *c *= i;
        }
        else if (auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            a->first *= i;
            a->second *= i;
        }
        return *this;
    }

    constexpr friend Bus operator*(Bus t, unsigned i) noexcept
    {
        t *= i;
        return t;
    }

    // operator!= is automatically generated
    constexpr friend bool operator==(const Bus&, const Bus&) noexcept = default;
private:
    Period mPeriod;
};
