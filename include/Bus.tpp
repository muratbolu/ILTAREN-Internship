#pragma once

#include "Chr.tpp"
#include "IO.tpp"
#include "LinkedList.tpp"

#include <cstdio>
#include <cstdlib>

class Bus
{
public:
    constexpr explicit Bus(int argc, char* argv[]) noexcept
    {
        if (argc != 4)
        {
            int rc = fputs("Provide exactly three arguments: number of buses, "
                           "start time, end time.\n",
                           stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
        }
        else
        {
            // NOLINTBEGIN
            mNumOfBuses = static_cast<unsigned>(atoi(argv[1]));
            mBeginTime = chr::Time { argv[2] };
            mEndTime = chr::Time { argv[3] };
            // NOLINTEND
            periods.pushBack(chr::Duration { 12 });
            printSchedule(stdout);
        }
    }

    constexpr void printSchedule(FILE* stream) const noexcept
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
    LinkedList<chr::Duration> periods;
};
