#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticStack.tpp"

#include <cstdio>
#include <cstdlib>

#define MAX_LINE_LEN 64
#define BP_POOL_SIZE 400
#define MAX_SAMPLES 2000

class BusPeriods
{
public:
    constexpr static BusPeriods* create(unsigned samplingPeriod) noexcept
    {
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
        for (unsigned i { 1 }; i < mLines.size(); ++i)
        {
            mSamples[(chr::Time { mLines[i]->data() } - begin).getDuration() / mSamplingPeriod - 1] = atoi(mLines[i]->data() + 6);
        }
        io::print(ostream, mSamples);
        fputc('\n', ostream);
    }
private:
    unsigned mSamplingPeriod;

    ObjectPool<Node<StaticStack<char, MAX_LINE_LEN>>, BP_POOL_SIZE> mLinePool;
    LinkedList<StaticStack<char, MAX_LINE_LEN>> mLines;

    StaticStack<unsigned, MAX_SAMPLES> mSamples;

    // Private constructor
    constexpr BusPeriods(unsigned samplingPeriod) noexcept :
        mSamplingPeriod { samplingPeriod }
    {
        mLines.pool() = &mLinePool;
    }
};
