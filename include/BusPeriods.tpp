#pragma once

#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticStack.tpp"

#include <cstdio>
#include <iostream>

#define MAX_LINE_LEN 64
#define BP_POOL_SIZE 400

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
        io::print(ostream, mLines);
        fputc('\n', ostream);
    }
private:
    unsigned mSamplingPeriod;

    ObjectPool<Node<StaticStack<char, MAX_LINE_LEN>>, BP_POOL_SIZE> mLinePool;
    LinkedList<StaticStack<char, MAX_LINE_LEN>> mLines;

    ObjectPool<Node<unsigned>, BP_POOL_SIZE> mSamplePool;
    LinkedList<unsigned> mSamples;

    // Private constructor
    constexpr BusPeriods(unsigned samplingPeriod) noexcept :
        mSamplingPeriod { samplingPeriod }
    {
        mSamples.pool() = &mSamplePool;
        mLines.pool() = &mLinePool;
    }
};
