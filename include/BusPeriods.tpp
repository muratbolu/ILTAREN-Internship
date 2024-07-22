#pragma once

#include "IO.tpp"
#include "LinkedList.tpp"
#include "Maybe.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <cstdio>

#define BP_POOL_SIZE 400

class BusPeriods
{
public:
    constexpr static Maybe<BusPeriods> create(unsigned samplingPeriod) noexcept
    {
        return just<BusPeriods>({ samplingPeriod });
    }

    void parseArrivals(FILE* istream, FILE* ostream) noexcept
    {
        mSamples.pushBack(5);
        mSamples.pushBack(7);
        // TODO: getline
        io::print(ostream, mSamples);
    }
private:
    unsigned mSamplingPeriod;
    ObjectPool<Node<unsigned>, BP_POOL_SIZE> mPool;
    LinkedList<unsigned> mSamples;

    // Private constructor
    constexpr BusPeriods(unsigned samplingPeriod) noexcept :
        mSamplingPeriod { samplingPeriod }
    {
        mSamples.pool() = &mPool;
    }
};
