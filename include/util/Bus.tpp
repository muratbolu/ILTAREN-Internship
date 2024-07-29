#pragma once

#include "util/Timer.tpp"

#include <cassert>
#include <compare>
#include <utility>
#include <variant>

class Bus
{
public:
    using ConstantPeriod = timer::Duration;
    using AlternatingPeriod = std::pair<ConstantPeriod, ConstantPeriod>;
    using Period = std::variant<ConstantPeriod, AlternatingPeriod>;

    constexpr explicit Bus(unsigned i = 0) noexcept :
        mPeriod { ConstantPeriod { i } }
    {
        assert(std::holds_alternative<ConstantPeriod>(mPeriod));
    }

    constexpr Bus(unsigned i, unsigned j) noexcept :
        mPeriod { std::make_pair(ConstantPeriod { i }, ConstantPeriod { j }) }
    {
        assert(i != 0);
        assert(j != 0);
        assert(i != j);
        assert((std::holds_alternative<AlternatingPeriod>(mPeriod)));
    }

    constexpr explicit Bus(ConstantPeriod i) noexcept :
        mPeriod { i }
    {
        assert(std::holds_alternative<ConstantPeriod>(mPeriod));
    }

    constexpr Bus(ConstantPeriod i, ConstantPeriod j) noexcept :
        mPeriod { std::make_pair(i, j) }
    {
        assert(i.getDuration() != 0);
        assert(j.getDuration() != 0);
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
            return c->getDuration();
        }
        if (const auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return a->first.getDuration();
        }
        return 0;
    }

    [[nodiscard]] constexpr unsigned getSecond() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return c->getDuration();
        }
        if (const auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return a->second.getDuration();
        }
        return 0;
    }

    [[nodiscard]] constexpr unsigned sum() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return c->getDuration() + c->getDuration();
        }
        if (const auto* a { std::get_if<AlternatingPeriod>(&mPeriod) })
        {
            return a->first.getDuration() + a->second.getDuration();
        }
        return 0;
    }

    [[nodiscard]] constexpr bool isAlternating() const noexcept
    {
        return std::holds_alternative<std::pair<ConstantPeriod, ConstantPeriod>>(mPeriod);
    }

    [[nodiscard]] constexpr bool isValid() const noexcept
    {
        if (const auto* c { std::get_if<ConstantPeriod>(&mPeriod) })
        {
            return *c != ConstantPeriod { 0 };
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

    // operators <, <=, >, >=  are automatically generated
    constexpr friend auto operator<=>(const Bus& lhs, const Bus& rhs) noexcept
    {
        unsigned lhsValue { lhs.sum() };
        unsigned rhsValue { rhs.sum() };
        if (!lhs.isAlternating())
        {
            lhsValue /= 2;
        }
        if (!rhs.isAlternating())
        {
            rhsValue /= 2;
        }
        // TODO: refactor, too ugly
        if (lhsValue == rhsValue)
        {
            if (lhs.isAlternating() && rhs.isAlternating())
            {
                return lhs.getFirst() <=> rhs.getFirst();
            }
            if (!lhs.isAlternating() && rhs.isAlternating())
            {
                return std::strong_ordering::less;
            }
            if (lhs.isAlternating() && !rhs.isAlternating())
            {
                return std::strong_ordering::greater;
            }
            if (!lhs.isAlternating() && !rhs.isAlternating())
            {
                return std::strong_ordering::equal;
            }
        }
        return lhsValue <=> rhsValue;
    }
private:
    Period mPeriod;
};
