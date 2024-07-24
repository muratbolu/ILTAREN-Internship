#include "BusAnalyzer.tpp"

#include <cstdlib>

constexpr static unsigned samplingPeriod { 1 };

int main(int argc, char* argv[])
{
    BusAnalyzer* bp { BusAnalyzer::create(argc, argv, samplingPeriod) };
    if (bp == nullptr)
    {
        return EXIT_FAILURE;
    }
    bp->getSamples();
    bp->parseSamples();
    bp->extractPeriods();
    // bp->extendSamplesToPowOfTwo();
    // bp->fastFourierTransform();
    return 0;
}
