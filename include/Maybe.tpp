#pragma once

#include <utility>

template<typename T>
class Maybe
{
public:
    constexpr Maybe(const Maybe&) noexcept = delete;
    constexpr Maybe(Maybe&&) noexcept = default;
    constexpr Maybe& operator=(const Maybe&) noexcept = delete;
    constexpr Maybe& operator=(Maybe&&) noexcept = default;

    template<typename U>
    constexpr friend Maybe<U> just(U&& t) noexcept;
    template<typename U>
    constexpr friend Maybe<U> nothing() noexcept;

    constexpr T data() const noexcept
    {
        return value;
    }

    constexpr T& data() noexcept
    {
        return value;
    }

    [[nodiscard]] constexpr bool exists() const noexcept
    {
        return mExists;
    }

    constexpr ~Maybe() noexcept
    {
        if (mExists)
        {
            value.~T();
        }
        mExists = false;
    }
private:
    union
    {
        T value;
    };

    bool mExists;

    constexpr Maybe(T&& data) noexcept :
        value { std::forward<T>(data) },
        mExists { true }
    {
    }

    constexpr Maybe() noexcept :
        mExists { false }
    {
    }
};

template<typename T>
constexpr Maybe<T> just(T&& t) noexcept
{
    return Maybe<T> { std::forward<T>(t) };
}

template<typename T>
constexpr Maybe<T> nothing() noexcept
{
    return Maybe<T> {};
}
