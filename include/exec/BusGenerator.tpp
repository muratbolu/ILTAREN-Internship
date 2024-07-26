#pragma once

#include "io/IO.tpp"
#include "sds/LinkedList.tpp"
#include "sds/Node.tpp"
#include "sds/ObjectPool.tpp"
#include "util/Bus.tpp"
#include "util/Timer.tpp"

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

    constexpr static BusGenerator* create(int argc, const char* const argv[], unsigned samplingPeriod) noexcept
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
        FILE* fw = fopen(argv[1], "w");
        if (fw == nullptr)
        {
            perror("Could not open file");
            return nullptr;
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
            fclose(fw);
            return nullptr;
        }
        return new BusGenerator { fw, numOfBuses, begin, end, samplingPeriod };
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
    void printArrivals() const noexcept
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

    [[nodiscard]] constexpr LinkedList<Bus>* getPeriods() noexcept
    {
        return &mPeriods;
    }
private:
    FILE* ostream { nullptr };
    unsigned mNumofBuses;
    Time mBeginTime, mEndTime;
    Dur mTotalDuration;
    unsigned mSamplingPeriod;
    ObjectPool<Node<Bus>, BS_POOL_SIZE> mPool;
    LinkedList<Bus> mPeriods;

    // Private constructor
    BusGenerator(FILE* fw, unsigned numOfBuses, Time begin, Time end, unsigned samplingPeriod) noexcept :
        ostream { fw },
        mNumofBuses { numOfBuses },
        mBeginTime { begin },
        mEndTime { end },
        mTotalDuration { end - begin },
        mSamplingPeriod { samplingPeriod },
        mPeriods { &mPool }
    {
        // Assign periods
        std::srand(std::time(nullptr));
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
