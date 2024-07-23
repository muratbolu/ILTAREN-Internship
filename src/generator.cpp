#include "BusSchedule.tpp"

#include <cstdio>
#include <cstdlib>

constexpr static unsigned samplingPeriod { 1 };

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
    return 0;
}
