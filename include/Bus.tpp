#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define POOL_SIZE 100

class Bus
{
public:
    constexpr explicit Bus(int argc, char* argv[]) noexcept
    {
        if (argc != 5)
        {
            int rc = fputs("Provide exactly four arguments: output file, number"
                           "of buses, begin time, end time.\n",
                           stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
            return;
        }
        // NOLINTBEGIN
        mNumOfBuses = static_cast<unsigned>(atoi(argv[2]));
        if (strlen(argv[3]) != 5 || strlen(argv[4]) != 5)
        {
            int rc = fputs("Enter valid timestamps, in the form of 00:00.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid timestamps");
            }
            return;
        }
        mBeginTime = chr::Time { argv[3] };
        mEndTime = chr::Time { argv[4] };
        // NOLINTEND
        if (mBeginTime >= mEndTime)
        {
            int rc = fputs("Begin time must be less than end time.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid timestamps");
            }
            return;
        }

        // Assign periods
        periods.pool() = &pool;
        for (unsigned i { 0 }; i < mNumOfBuses; ++i)
        {
            periods.pushBack(chr::Duration { 20 + 10 * i });
        }

        // Print data members info
        // printSchedule(stdout);

        FILE* fp = fopen(argv[1], "w");   // NOLINT
        if (fp == nullptr)
        {
            perror("Could not open file");
            return;
        }
        printArrivals(fp);
    }

    void printSchedule(FILE* stream) const noexcept
    {
        fprintf(stream, "buses: %d\n", mNumOfBuses);
        io::print(stream, periods);
        fprintf(stream, "begin: ");
        io::print(stream, mBeginTime);
        fprintf(stream, "  end: ");
        io::print(stream, mEndTime);
    }

    // TODO: given the output of this func, deduce the periods of buses
    void printArrivals(FILE* stream) const noexcept
    {
        for (chr::Duration d { 10 }; mBeginTime + d <= mEndTime; d += 10)
        {
            unsigned num { 0 };
            for (auto&& p : periods)
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
                fputs("\n", stream);
            }
        }
    }
private:
    unsigned mNumOfBuses { 0 };
    chr::Time mBeginTime, mEndTime;
    ObjectPool<Node<chr::Duration>, POOL_SIZE> pool;
    LinkedList<chr::Duration> periods;
};
