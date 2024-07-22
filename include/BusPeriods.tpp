#pragma once

#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticVector.tpp"

#include <cstdio>

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
        mSamples.pushBack(5);
        mSamples.pushBack(7);

        /*
        for (;;)
        {
            mLines.pushBack(StaticVector<char, MAX_LINE_LEN> {});
            if (fgets(mLines.back()->data(), MAX_LINE_LEN, istream) == nullptr)
            {
                break;
            }
        }
        */

        io::print(ostream, mSamples);
    }
private:
    unsigned mSamplingPeriod;

    ObjectPool<Node<StaticVector<char, MAX_LINE_LEN>>, BP_POOL_SIZE> mLinePool;
    LinkedList<StaticVector<char, MAX_LINE_LEN>> mLines;

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
