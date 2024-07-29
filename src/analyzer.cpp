#include "exec/BusAnalyzer.tpp"

#include <cstdlib>
#include <optional>

constexpr static unsigned samplingPeriod { 1 };

int main(int argc, const char* argv[])
{
    std::optional<BusAnalyzer> ba { BusAnalyzer::create(argc, argv, samplingPeriod) };
    if (!ba.has_value())
    {
        return EXIT_FAILURE;
    }
    ba->getSamples();
    ba->parseSamples();
    ba->extractPeriods();
    ba->printResult();
    return 0;
}
