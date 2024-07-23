#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define BS_POOL_SIZE 500

class BusSchedule
{
    using Time = chr::Time;
    using Dur = chr::Duration;
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
        Time begin { Time { argv[3] } };
        Time end { Time { argv[4] } };
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
        for (Dur d { mSamplingPeriod }; mBeginTime + d <= mEndTime; d += mSamplingPeriod)
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
    Time mBeginTime, mEndTime;
    unsigned mSamplingPeriod;
    ObjectPool<Node<Dur>, BS_POOL_SIZE> mPool;
    LinkedList<Dur> mPeriods;

    // Private constructor
    constexpr BusSchedule(unsigned numOfBuses, Time begin, Time end, unsigned samplingPeriod) noexcept :
        mNumofBuses { numOfBuses },
        mBeginTime { begin },
        mEndTime { end },
        mSamplingPeriod { samplingPeriod }
    {
        // Assign periods
        mPeriods.pool() = &mPool;
        std::srand(std::time(nullptr));
        for (unsigned i { 0 }; i < mNumofBuses; ++i)
        {
            unsigned maxNumber { 60 };
            unsigned minNumber { 1 };
            unsigned randNum { static_cast<unsigned>(std::rand()) % (maxNumber + 1 - minNumber) + minNumber };
            mPeriods.pushBack(Dur { randNum });
        }
        io::print(stdout, mPeriods);
        fputc('\n', stdout);
    }
};
