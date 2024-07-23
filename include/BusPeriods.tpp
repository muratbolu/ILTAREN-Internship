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

#define MAX_LINE_LEN 64
#define BP_POOL_SIZE 400
#define MAX_SAMPLES 2000

class BusPeriods
{
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
        return new BusPeriods { samplingPeriod };
    }

    void parseArrivals(FILE* istream, FILE* ostream) noexcept
    {
        // Assume that EOF is not zero in the implementation
        for (int c { 0 }; c != EOF;)
        {
            mLines.pushBack(StaticStack<char, MAX_LINE_LEN> {});
            StaticStack<char, MAX_LINE_LEN>* back { mLines.back() };
            for (;;)
            {
                c = fgetc(istream);
                if (c == EOF || c == '\n')
                {
                    break;
                }
                back->pushBack(static_cast<char>(c));
            }
            if (c == EOF)
            {
                mLines.popBack();
            }
        }
        // The output start from the smallest sampling period.
        // For example, if begin is 12:00 and SP=10, samples
        // start from 12:10.
        chr::Time begin { mLines.front()->data() };
        chr::Time end { mLines.back()->data() };
        for (chr::Duration d { mSamplingPeriod }; begin + d <= end; d += mSamplingPeriod)
        {
            mSamples.pushBack(0);
        }
        for (unsigned i { 0 }; i < mLines.size(); ++i)
        {
            mSamples[(chr::Time { mLines[i]->data() } - begin).getDuration() / mSamplingPeriod] = atoi(mLines[i]->data() + 6);
        }
        io::print(ostream, mSamples);
        fputc('\n', ostream);
        for (;;)
        {
            mFreqs.pushBack(extractHighestFreq(mSamples) * mSamplingPeriod);
            if (mFreqs.back() == 0)
            {
                mFreqs.popBack();
                break;
            }
        }
        io::print(ostream, mFreqs);
        fputc('\n', ostream);
        // TODO: separate the synthesizer and analyzer programs
        /*
        while (((mSamples.size()) & (mSamples.size() - 1)) != 0)
        {
            mSamples.pushBack(0);
        }
        io::print(ostream, mSamples);
        fputc('\n', ostream);
        for (unsigned i { 0 }; i < mSamples.size(); ++i)
        {
            mComplexSamples.pushBack(std::complex<float> { static_cast<float>(mSamples[i]), 0 });
        }
        io::print(ostream, mComplexSamples);
        fputc('\n', ostream);
        mFourierTransform = fft(mComplexSamples);
        io::print(ostream, mFourierTransform);
        fputc('\n', ostream);
        for (unsigned i { 0 }; i < mFourierTransform.size(); ++i)
        {
            mAbsFT.pushBack(std::abs(mFourierTransform[i]));
        }
        io::print(ostream, mAbsFT);
        fputc('\n', ostream);
        */
    }

    /*
    constexpr static StaticStack<std::complex<float>, MAX_SAMPLES> fft(const StaticStack<std::complex<float>, MAX_SAMPLES>& p) noexcept
    {
        if (p.size() == 1)
        {
            return p;
        }
        std::complex<float> omega { 1, static_cast<float>(2 * PI / p.size()) };
        StaticStack<std::complex<float>, MAX_SAMPLES> pe;
        StaticStack<std::complex<float>, MAX_SAMPLES> po;
        for (unsigned i { 0 }; i + 1 < p.size(); i += 2)
        {
            pe.pushBack(p[i]);
            po.pushBack(p[i + 1]);
        }

        StaticStack<std::complex<float>, MAX_SAMPLES> ye { fft(pe) };
        StaticStack<std::complex<float>, MAX_SAMPLES> yo { fft(po) };

        StaticStack<std::complex<float>, MAX_SAMPLES> y;
        for (unsigned i { 0 }; i < p.size(); ++i)
        {
            y.pushBack(std::complex<float> { 0, 0 });
        }
        for (unsigned i { 0 }; i < p.size() / 2; ++i)
        {
            std::complex<float> omegaJ { std::pow(omega, i) };
            y[i] = ye[i] + omegaJ * yo[i];
            y[i + p.size() / 2] = ye[i] - omegaJ * yo[i];
        }
        return y;
    }
    */
    constexpr unsigned extractHighestFreq(StaticStack<unsigned, MAX_SAMPLES>& s) noexcept
    {
        unsigned period { 0 };
        for (unsigned i { 0 }; i < s.size(); ++i)
        {
            if (s[i] > 0)
            {
                assert(s[i] == 1);
                period = i;
                break;
            }
        }
        if (period == 0)
        {
            return 0;
        }
        for (unsigned i { period }; i < s.size(); i += period)
        {
            --s[i];
        }
        return period;
    }
private:
    unsigned mSamplingPeriod;

    ObjectPool<Node<StaticStack<char, MAX_LINE_LEN>>, BP_POOL_SIZE> mLinePool;
    LinkedList<StaticStack<char, MAX_LINE_LEN>> mLines;

    StaticStack<unsigned, MAX_SAMPLES> mSamples;
    StaticStack<unsigned, MAX_SAMPLES> mFreqs;

    // StaticStack<std::complex<float>, MAX_SAMPLES> mComplexSamples;
    // StaticStack<std::complex<float>, MAX_SAMPLES> mFourierTransform;
    // StaticStack<float, MAX_SAMPLES> mAbsFT;

    // Private constructor
    constexpr BusPeriods(unsigned samplingPeriod) noexcept :
        mSamplingPeriod { samplingPeriod }
    {
        mLines.pool() = &mLinePool;
    }
};
