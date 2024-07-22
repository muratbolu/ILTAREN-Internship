#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"
#include "Maybe.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define POOL_SIZE 500

class BusSchedule
{
public:
    constexpr static Maybe<BusSchedule> create(int argc, char* argv[]) noexcept
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
            return nothing<BusSchedule>();
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
            return nothing<BusSchedule>();
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
            return nothing<BusSchedule>();
        }
        return just<BusSchedule>({ numOfBuses, begin, end });
    }

    void printSchedule(FILE* stream) const noexcept
    {
        fprintf(stream, "Buses: %d\n", mNumofBuses);
        io::print(stream, periods);
        fprintf(stream, "begin: ");
        io::print(stream, mBeginTime);
        fprintf(stream, "  end: ");
        io::print(stream, mEndTime);
    }

    // TODO: given the output of this func, deduce the periods of BusSchedulees
    void printArrivals(FILE* stream) const noexcept
    {
        for (chr::Duration d { 0 }; mBeginTime + d <= mEndTime; d += 10)
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

    void parseArrivals(FILE* stream) noexcept
    {
        // TODO
    }
private:
    unsigned mNumofBuses;
    chr::Time mBeginTime, mEndTime;
    ObjectPool<Node<chr::Duration>, POOL_SIZE> pool;
    LinkedList<chr::Duration> periods;
    LinkedList<chr::Duration> deducedPeriods;

    // Private constructor
    constexpr BusSchedule(unsigned numOfBuses, chr::Time begin, chr::Time end) noexcept :
        mNumofBuses { numOfBuses },
        mBeginTime { begin },
        mEndTime { end }
    {
        // Assign periods
        periods.pool() = &pool;
        deducedPeriods.pool() = &pool;
        for (unsigned i { 0 }; i < mNumofBuses; ++i)
        {
            periods.pushBack(chr::Duration { 20 + 10 * i });
        }
    }
};
