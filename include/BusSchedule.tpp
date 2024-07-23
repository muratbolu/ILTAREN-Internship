#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BS_POOL_SIZE 500

class BusSchedule
{
public:
    constexpr BusSchedule() noexcept = delete;
    constexpr ~BusSchedule() noexcept = default;
    constexpr BusSchedule(const BusSchedule&) noexcept = delete;
    constexpr BusSchedule(BusSchedule&&) noexcept = default;
    constexpr BusSchedule& operator=(const BusSchedule&) noexcept = delete;
    constexpr BusSchedule& operator=(BusSchedule&&) noexcept = default;

    constexpr static BusSchedule* create(int argc, char* argv[], unsigned samplingPeriod) noexcept
    {
        if (argc != 5)
        {
            int rc = fputs("Provide exactly four arguments: output file, number"
                           " of buses, begin time, end time.\n",
                           stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
            return nullptr;
        }
        // NOLINTBEGIN
        unsigned numOfBuses { static_cast<unsigned>(atoi(argv[2])) };
        if (strlen(argv[3]) != 5 || strlen(argv[4]) != 5)
        {
            int rc = fputs("Enter valid timestamps, in the form of 00:00.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid timestamps");
            }
            return nullptr;
        }
        chr::Time begin { chr::Time { argv[3] } };
        chr::Time end { chr::Time { argv[4] } };
        // NOLINTEND
        if (begin >= end)
        {
            int rc = fputs("Begin time must be less than end time.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid timestamps");
            }
            return nullptr;
        }
        return new BusSchedule { numOfBuses, begin, end, samplingPeriod };
    }

    void printSchedule(FILE* stream) const noexcept
    {
        fprintf(stream, "Buses: %d\n", mNumofBuses);
        io::print(stream, mPeriods);
        fprintf(stream, "begin: ");
        io::print(stream, mBeginTime);
        fprintf(stream, "  end: ");
        io::print(stream, mEndTime);
    }

    // TODO: given the output of this func, deduce the periods of BusSchedulees
    void printArrivals(FILE* stream) const noexcept
    {
        io::print(stream, mBeginTime);
        fputc('\n', stream);
        for (chr::Duration d { mSamplingPeriod }; mBeginTime + d <= mEndTime; d += mSamplingPeriod)
        {
            unsigned num { 0 };
            for (auto&& p : mPeriods)
            {
                if ((d.getDuration() % p.getDuration()) == 0U)
                {
                    ++num;
                }
            }
            if (num > 0)
            {
                io::print(stream, mBeginTime + d);
                fprintf(stream, " %d", num);
                fputc('\n', stream);
            }
        }
    }
private:
    unsigned mNumofBuses;
    chr::Time mBeginTime, mEndTime;
    unsigned mSamplingPeriod;
    ObjectPool<Node<chr::Duration>, BS_POOL_SIZE> mPool;
    LinkedList<chr::Duration> mPeriods;

    // Private constructor
    constexpr BusSchedule(unsigned numOfBuses, chr::Time begin, chr::Time end, unsigned samplingPeriod) noexcept :
        mNumofBuses { numOfBuses },
        mBeginTime { begin },
        mEndTime { end },
        mSamplingPeriod { samplingPeriod }
    {
        // Assign periods
        mPeriods.pool() = &mPool;
        for (unsigned i { 0 }; i < mNumofBuses; ++i)
        {
            mPeriods.pushBack(chr::Duration { 20 + 10 * i });
        }
    }
};
