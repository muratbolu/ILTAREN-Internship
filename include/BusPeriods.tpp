#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticStack.tpp"

#include <cassert>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

#define MAX_LINE_LEN 64
#define BP_POOL_SIZE 400
#define MAX_SAMPLES 2000

#define IS_POW_TWO(x) (((x) & ((x) - 1)) == 0)

class BusPeriods
{
    using Time = chr::Time;
    using Dur = chr::Duration;
public:
    constexpr static BusPeriods* create(int argc, char* argv[], unsigned samplingPeriod) noexcept
    {
        if (argc != 2)
        {
            int rc = fputs("Provide exactly one argument: input file.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
            return nullptr;
        }
        FILE* fr = fopen(argv[1], "r");   // NOLINT
        if (fr == nullptr)
        {
            perror("Could not open file");
            return nullptr;
        }
        return new BusPeriods { fr, stdout, samplingPeriod };
    }

    void getSamples() noexcept
    {
        // Assume that EOF is not zero in the implementation
        for (;;)
        {
            mLines.pushBack(StaticStack<char, MAX_LINE_LEN> {});
            StaticStack<char, MAX_LINE_LEN>* back { mLines.back() };
            int c { EOF };
            for (;;)
            {
                c = fgetc(istream);
                if (c == EOF || c == '\n')
                {
                    back->pushBack('\0');
                    break;
                }
                back->pushBack(static_cast<char>(c));
            }
            if (c == EOF)
            {
                mLines.popBack();
                break;
            }
        }
    }

    void parseSamples() noexcept
    {
        assert(strlen(mLines.front()->data()) == 5);
        Time begin { mLines.front()->data() };
        assert(strlen(mLines.back()->data()) > 6);
        Time end { mLines.back()->data() };
        for (Dur d { mSP }; begin + d <= end; d += mSP)
        {
            mSamples.pushBack(0);
        }
        for (auto&& l : mLines)
        {
            if (strlen(l.data()) < 7)
            {
                continue;
            }
            mSamples[(Time { l.data() } - begin).getDuration() / mSP] = atoi(l.data() + 6);
        }
        // io::print(ostream, mSamples);
        // fputc('\n', ostream);
    }

    void extractPeriods() noexcept
    {
        StaticStack<unsigned, MAX_SAMPLES> mFreqs;
        StaticStack<unsigned, MAX_SAMPLES> samples { mSamples };
        for (;;)
        {
            unsigned t { extractConstantPeriod(samples) };
            if (t == 0)
            {
                break;
            }
            mFreqs.pushBack(t * mSP);
            for (unsigned i { t }; i < samples.size(); i += t)
            {
                --samples[i];
            }
        }
        fprintf(ostream, "Length: %d\n", mFreqs.size());
        io::print(ostream, mFreqs);
        fputc('\n', ostream);
    }

    [[nodiscard]] static unsigned extractConstantPeriod(const StaticStack<unsigned, MAX_SAMPLES>& s) noexcept
    {
        for (unsigned i { 1 }; i < s.size() / 2; ++i)
        {
            unsigned period { 0 };
            for (unsigned j { i }; j < s.size() / 2; ++j)
            {
                if (s[j] > 0)
                {
                    period = j;
                    break;
                }
            }
            if (period == 0)
            {
                return 0;
            }
            bool isValid { true };
            for (unsigned j { period }; j < s.size(); j += period)
            {
                if (s[j] == 0)
                {
                    isValid = false;
                }
            }
            if (isValid)
            {
                return period;
            }
        }
        return 0;
    }

    /*
    [[nodiscard]] constexpr static StaticStack<unsigned, MAX_SAMPLES> shiftIndex(const StaticStack<unsigned, MAX_SAMPLES>& s, unsigned n) noexcept
    {
        StaticStack<unsigned, MAX_SAMPLES> shifted;
        for (unsigned i { n }; i < s.size(); ++i)
        {
            shifted.pushBack(s[i]);
        }
        return shifted;
    }

    [[nodiscard]] constexpr static std::pair<unsigned, unsigned> extractAlternatingPeriod(const StaticStack<unsigned, MAX_SAMPLES>& s) noexcept
    {
        unsigned period1 { 0 };
        for (unsigned i { 1 }; i < s.size(); ++i)
        {
            if (s[i] > 0)
            {
                // Some inputs may be duplicated.
                // assert(s[i] == 1);
                period1 = i;
                break;
            }
        }
        if (period1 == 0)
        {
            return { 0, 0 };
        }
        io::print(stdout, shiftIndex(s, period1));
        fprintf(stdout, "found period: %d\n", extractConstantPeriod(shiftIndex(s, period1)));
        return { period1, period1 };
    }
    */

    void extendSamplesToPowOfTwo() noexcept
    {
        while (!IS_POW_TWO(mSamples.size()))
        {
            mSamples.pushBack(0);
        }
        // io::print(ostream, mSamples);
        // fputc('\n', ostream);
    }

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
        // io::print(ostream, mAbsFT);
        // fputc('\n', ostream);
    }

    [[nodiscard]] constexpr StaticStack<std::complex<float>, MAX_SAMPLES> fft(const StaticStack<std::complex<float>, MAX_SAMPLES>& p) const noexcept
    {
        unsigned n { p.size() };
        if (n == 1)
        {
            return p;
        }
        StaticStack<std::complex<float>, MAX_SAMPLES> pe;
        StaticStack<std::complex<float>, MAX_SAMPLES> po;

        assert(IS_POW_TWO(n));
        for (unsigned i { 0 }; i + 1 < n; i += 2)
        {
            assert(i % 2 == 0);
            pe.pushBack(p[i]);
            po.pushBack(p[i + 1]);
        }
        assert(pe.size() == po.size());

        StaticStack<std::complex<float>, MAX_SAMPLES> ye { fft(pe) };
        StaticStack<std::complex<float>, MAX_SAMPLES> yo { fft(po) };

        StaticStack<std::complex<float>, MAX_SAMPLES> y;
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
private:
    // Input and output files
    FILE *istream, *ostream;
    // Sampling period
    unsigned mSP;

    ObjectPool<Node<StaticStack<char, MAX_LINE_LEN>>, BP_POOL_SIZE> mLinePool;
    // Input lines
    LinkedList<StaticStack<char, MAX_LINE_LEN>> mLines;

    StaticStack<unsigned, MAX_SAMPLES> mSamples;

    StaticStack<std::complex<float>, MAX_SAMPLES> mComplexSamples;
    StaticStack<std::complex<float>, MAX_SAMPLES> mFourierTransform;
    StaticStack<float, MAX_SAMPLES> mAbsFT;

    // Private constructor
    constexpr BusPeriods(FILE* is, FILE* os, unsigned samplingPeriod) noexcept :
        istream { is },
        ostream { os },
        mSP { samplingPeriod }
    {
        mLines.pool() = &mLinePool;
    }
};
