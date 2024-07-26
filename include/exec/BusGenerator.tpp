#pragma once

#include "exec/BusGenerator.tpp"
#include "io/IO.tpp"
#include "sds/LinkedList.tpp"
#include "sds/Node.tpp"
#include "sds/ObjectPool.tpp"
#include "sds/StaticStack.tpp"
#include "util/Bus.tpp"
#include "util/Timer.tpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <optional>

#ifndef BUS_POOL_SIZE
#define BUS_POOL_SIZE 500
#endif

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

    constexpr static std::optional<BusGenerator> create(int argc, const char* const argv[], unsigned samplingPeriod) noexcept
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
            return std::nullopt;
        }
        // NOLINTBEGIN
        FILE* fw = fopen(argv[1], "w");
        if (fw == nullptr)
        {
            perror("Could not open file");
            return std::nullopt;
        }
        unsigned numOfBuses { static_cast<unsigned>(atoi(argv[2])) };
        if (strlen(argv[3]) != 5 || strlen(argv[4]) != 5)
        {
            int rc = fputs("Enter valid timestamps, in the form of 00:00.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid timestamps");
            }
            fclose(fw);
            return std::nullopt;
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
            fclose(fw);
            return std::nullopt;
        }
        return std::make_optional<BusGenerator>(BusGenerator { fw, numOfBuses, begin, end, samplingPeriod });
    }

    void printSchedule(FILE* os = stdout) noexcept
    {
        fputs("Buses: ", os);
        io::print(os, mNumofBuses);
        fputc('\n', os);
        mPeriods.sort();
        io::print(os, mPeriods);
        fputc('\n', os);
        fputs("begin: ", os);
        io::print(os, mBeginTime);
        fputc('\n', os);
        fputs("  end: ", os);
        io::print(os, mEndTime);
        fputc('\n', os);
    }

    // TODO: given the output of this func, deduce the periods of BusSchedulees
    void printArrivals() noexcept
    {
        io::print(ostream, mBeginTime);
        fputc('\n', ostream);
        for (Dur d { mSamplingPeriod }; mBeginTime + d <= mEndTime; d += mSamplingPeriod)
        {
            unsigned num { 0 };
            for (auto& p : mPeriods)
            {
                unsigned sum { (p.getFirst() + p.getSecond()) };
                if ((d.getDuration() % sum) == 0U)
                {
                    ++num;
                }
                else if (((d.getDuration() + p.getSecond()) % sum) == 0U)
                {
                    ++num;
                }
            }
            if (num > 0)
            {
                io::print(ostream, mBeginTime + d);
                fputc(' ', ostream);
                io::print(ostream, num);
                fputc('\n', ostream);
            }
        }
        fflush(ostream);
    }

    [[nodiscard]] constexpr StaticStack<Bus, BUS_POOL_SIZE>& getPeriods() noexcept
    {
        return mPeriods;
    }
private:
    FILE* ostream { nullptr };
    unsigned mNumofBuses;
    Time mBeginTime, mEndTime;
    Dur mTotalDuration;
    unsigned mSamplingPeriod;
    StaticStack<Bus, BUS_POOL_SIZE> mPeriods;

    // Private constructor
    BusGenerator(FILE* fw, unsigned numOfBuses, Time begin, Time end, unsigned samplingPeriod) noexcept :
        ostream { fw },
        mNumofBuses { numOfBuses },
        mBeginTime { begin },
        mEndTime { end },
        mTotalDuration { end - begin },
        mSamplingPeriod { samplingPeriod }
    {
        // Assign periods
        // std::srand(std::time(nullptr));
        std::srand(0);
        for (unsigned i { 0 }; i < mNumofBuses;)
        {
            constexpr unsigned maxNumber { 60 };
            constexpr unsigned minNumber { 1 };

            // Pick a random number between 1 and 60.
            unsigned randNum1 { std::rand() % (maxNumber + 1 - minNumber) + minNumber };
            // Set the second number to be the same as first.
            unsigned randNum2 { randNum1 };

            // %50 chance to have an alternating period.
            if (std::rand() % 2)
            {
                // Change the second number to be a different number.
                randNum2 = std::rand() % (maxNumber + 1 - minNumber) + minNumber;
            }

            bool isValid { true };
            if (randNum1 == randNum2 && randNum1 + randNum2 > mTotalDuration.getDuration())
            {
                isValid = false;
            }
            else if (randNum1 != randNum2 && 2 * (randNum1 + randNum2) > mTotalDuration.getDuration())
            {
                isValid = false;
            }
            if (isValid)
            {
                if (randNum1 == randNum2)
                {
                    mPeriods.pushBack(Bus { randNum1 });
                }
                else
                {
                    mPeriods.pushBack(Bus { randNum1, randNum2 });
                }
                ++i;
            }
        }
        // io::print(stdout, mPeriods);
        // fputc('\n', stdout);
    }
};
