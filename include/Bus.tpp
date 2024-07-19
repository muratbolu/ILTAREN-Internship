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
        if (argc != 4)
        {
            int rc = fputs("Provide exactly three arguments: number of buses, "
                           "begin time, end time.\n",
                           stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
            return;
        }
        // NOLINTBEGIN
        mNumOfBuses = static_cast<unsigned>(atoi(argv[1]));
        if (strlen(argv[2]) != 5 || strlen(argv[3]) != 5)
        {
            int rc = fputs("Enter valid timestamps, in the form of 00:00.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid timestamps");
            }
            return;
        }
        mBeginTime = chr::Time { argv[2] };
        mEndTime = chr::Time { argv[3] };
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
        printSchedule(stdout);
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
private:
    unsigned mNumOfBuses { 0 };
    chr::Time mBeginTime, mEndTime;
    ObjectPool<Node<chr::Duration>, POOL_SIZE> pool;
    LinkedList<chr::Duration> periods;
};
