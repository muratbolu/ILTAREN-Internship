#pragma once

#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "Timer.tpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <utility>

#define BS_POOL_SIZE 500

class BusGenerator
{
    using Time = timer::Time;
    using Dur = timer::Duration;
public:
    constexpr BusGenerator() noexcept = delete;
    constexpr ~BusGenerator() noexcept = default;
    constexpr BusGenerator(const BusGenerator&) noexcept = delete;
    constexpr BusGenerator(BusGenerator&&) noexcept = default;
    constexpr BusGenerator& operator=(const BusGenerator&) noexcept = delete;
    constexpr BusGenerator& operator=(BusGenerator&&) noexcept = default;

    constexpr static BusGenerator* create(int argc, char* argv[], unsigned samplingPeriod) noexcept
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
        return new BusGenerator { numOfBuses, begin, end, samplingPeriod };
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
            for (auto& p : mPeriods)
            {
                unsigned sum { (p.first.getDuration() + p.second.getDuration()) };
                if ((d.getDuration() % sum) == 0U)
                {
                    ++num;
                }
                else if (((d.getDuration() + p.second.getDuration()) % sum) == 0U)
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
    Dur mTotalDuration;
    unsigned mSamplingPeriod;
    ObjectPool<Node<std::pair<Dur, Dur>>, BS_POOL_SIZE> mPool;
    LinkedList<std::pair<Dur, Dur>> mPeriods;

    // Private constructor
    BusGenerator(unsigned numOfBuses, Time begin, Time end, unsigned samplingPeriod) noexcept :
        mNumofBuses { numOfBuses },
        mBeginTime { begin },
        mEndTime { end },
        mTotalDuration { end - begin },
        mSamplingPeriod { samplingPeriod }
    {
        // Assign periods
        mPeriods.pool() = &mPool;
        std::srand(std::time(nullptr));
        for (unsigned i { 0 }; i < mNumofBuses;)
        {
            unsigned maxNumber { 60 };
            unsigned minNumber { 1 };
            unsigned randNum1 { static_cast<unsigned>(std::rand()) % (maxNumber + 1 - minNumber) + minNumber };
            unsigned randNum2 { randNum1 };
            if (std::rand() % 2)
            {
                randNum2 = static_cast<unsigned>(std::rand()) % (maxNumber + 1 - minNumber) + minNumber;
            }
            bool isValid { true };
            /*
            if (randNum1 == randNum2 && randNum1 + randNum2 > mTotalDuration.getDuration())
            {
                isValid = false;
            }
            else if (randNum1 != randNum2 && 2 * (randNum1 + randNum2) > mTotalDuration.getDuration())
            {
                isValid = false;
            }
            else
            {
                for (auto&& p : mPeriods)
                {
                    if (p.first == randNum1 || p.first == randNum2 || p.second == randNum1 || p.second == randNum2 ||
                        p.first + p.second == randNum1 || p.first + p.second == randNum2 || p.first + p.second == randNum1 + randNum2)
                    {
                        isValid = false;
                        break;
                    }
                }
            }
            */
            if (isValid)
            {
                mPeriods.pushBack({ Dur { randNum1 }, Dur { randNum2 } });
                ++i;
            }
        }
        io::print(stdout, mPeriods);
        fputc('\n', stdout);
    }
};
