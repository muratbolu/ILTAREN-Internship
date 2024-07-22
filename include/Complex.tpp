#pragma once

#define PI 3.14159265359

class Complex
{
public:
    constexpr Complex() noexcept = default;

    constexpr Complex(float r, float theta) noexcept :
        mR { r },
        mTheta { theta }
    {
    }

    [[nodiscard]] constexpr float getRadius() const noexcept
    {
        return mR;
    }

    constexpr float& setRadius() noexcept
    {
        return mR;
    }

    [[nodiscard]] constexpr float getAngle() const noexcept
    {
        return mTheta;
    }

    constexpr float& setAngle() noexcept
    {
        return mTheta;
    }

    constexpr inline Complex& operator*=(const Complex& rhs) noexcept
    {
        mR *= rhs.mR;
        mTheta += rhs.mTheta;
        return *this;
    }

    constexpr friend inline Complex operator*(Complex lhs, const Complex& rhs) noexcept
    {
        lhs *= rhs;
        return lhs;
    }

    // operator!= is automatically generated
    constexpr friend inline bool operator==(const Complex& lhs, const Complex& rhs) noexcept = default;
private:
    float mR { 0 }, mTheta { 0 };
};
