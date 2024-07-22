// TRAVELER or BUS
#include <cstdlib>
#define BUS

#ifdef TRAVELER
#include "Traveler.tpp"

int main(int argc, char* argv[])
{
    Traveler t { argc, argv };
    return 0;
}
#endif

#ifdef BUS
#include "BusPeriods.tpp"
#include "BusSchedule.tpp"

#include <cstdio>

constexpr static unsigned samplingPeriod { 10 };

int main(int argc, char* argv[])
{
    BusSchedule* bs { BusSchedule::create(argc, argv, samplingPeriod) };
    if (bs == nullptr)
    {
        return EXIT_FAILURE;
    }
    FILE* fw = fopen(argv[1], "w");   // NOLINT
    if (fw == nullptr)
    {
        perror("Could not open file");
        return EXIT_FAILURE;
    }
    bs->printArrivals(fw);

    BusPeriods* bp { BusPeriods::create(samplingPeriod) };
    if (bp == nullptr)
    {
        return EXIT_FAILURE;
    }
    FILE* fr = fopen(argv[1], "r");   // NOLINT
    if (fr == nullptr)
    {
        perror("Could not open file");
        return EXIT_FAILURE;
    }
    bp->parseArrivals(fr, stdout);

    return 0;
}
#endif
