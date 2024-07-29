#include "exec/BusGenerator.tpp"

#include <cstdlib>
#include <optional>

constexpr static unsigned samplingPeriod { 1 };

int main(int argc, const char* argv[])
{
    std::optional<BusGenerator> bg { BusGenerator::create(argc, argv, samplingPeriod) };
    if (!bg.has_value())
    {
        return EXIT_FAILURE;
    }
    bg->printArrivals();
    bg->printInfo();
    return 0;
}
