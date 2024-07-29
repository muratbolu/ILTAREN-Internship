#pragma once

#include "sds/StaticStack.tpp"

#include <cassert>
#include <cmath>
#include <complex>

#define IS_POW_TWO(x) (((x) & ((x) - 1)) == 0)

namespace fft
{
template<unsigned N>
void extendSamplesToPowOfTwo(StaticStack<unsigned, N>& stack) noexcept
{
    while (!IS_POW_TWO(stack.size()))
    {
        stack.pushBack(0);
    }
}

/*
void fastFourierTransform() noexcept
{
    for (unsigned i { 0 }; i < mSamples.size(); ++i)
    {
        mComplexSamples.pushBack(std::complex<float> { static_cast<float>(mSamples[i]), 0 });
    }
    // io::print(ostream, mComplexSamples);
    // fputc('\n', ostream);
    // fputc('\n', ostream);

    mFourierTransform = fft(mComplexSamples);
    // io::print(ostream, mFourierTransform);
    // fputc('\n', ostream);
    // fputc('\n', ostream);

    for (unsigned i { 0 }; i < mFourierTransform.size(); ++i)
    {
        mAbsFT.pushBack(std::abs(mFourierTransform[i]));
    }
    io::print(ostream, mAbsFT);
    fputc('\n', ostream);
}
*/

template<unsigned N>
[[nodiscard]] constexpr StaticStack<std::complex<float>, N> fft(const StaticStack<std::complex<float>, N>& p) noexcept
{
    unsigned n { p.size() };
    if (n == 1)
    {
        return p;
    }
    StaticStack<std::complex<float>, N> pe;
    StaticStack<std::complex<float>, N> po;

    assert(IS_POW_TWO(n));
    for (unsigned i { 0 }; i + 1 < n; i += 2)
    {
        assert(i % 2 == 0);
        pe.pushBack(p[i]);
        po.pushBack(p[i + 1]);
    }
    assert(pe.size() == po.size());

    StaticStack<std::complex<float>, N> ye { fft(pe) };
    StaticStack<std::complex<float>, N> yo { fft(po) };

    StaticStack<std::complex<float>, N> y;
    for (unsigned i { 0 }; i < n; ++i)
    {
        y.pushBack(std::complex<float> { 0, 0 });
    }
    const float pi = std::acosf(-1.0);
    const std::complex<float> i(0.0, 1.0);
    std::complex<float> omega { std::exp(2 * pi * i / std::complex<float> { static_cast<float>(n), 0 }) };
    for (unsigned j { 0 }; j < n / 2; ++j)
    {
        const std::complex<float> omegaJ { std::pow(omega, j) };
        y[j] = ye[j] + omegaJ * yo[j];
        y[j + n / 2] = ye[j] - omegaJ * yo[j];
    }
    return y;
}
};   // namespace fft
