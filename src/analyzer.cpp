#include "BusPeriods.tpp"

#include <cstdio>
#include <cstdlib>

constexpr static unsigned samplingPeriod { 10 };

int main(int argc, char* argv[])
{
    BusPeriods* bp { BusPeriods::create(argc, argv, samplingPeriod) };
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
