#include "exec/BusGenerator.tpp"

#include <cstdlib>

constexpr static unsigned samplingPeriod { 1 };

int main(int argc, const char* argv[])
{
    BusGenerator* bs { BusGenerator::create(argc, argv, samplingPeriod) };
    if (bs == nullptr)
    {
        return EXIT_FAILURE;
    }
    bs->printArrivals();
    return 0;
}
