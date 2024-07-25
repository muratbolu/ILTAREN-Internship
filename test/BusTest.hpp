#pragma once

#include "exec/BusAnalyzer.tpp"
#include "exec/BusGenerator.tpp"
#include "sds/LinkedList.tpp"
#include "util/Bus.tpp"

#include <gtest/gtest.h>

constexpr const char* const fileName { "./temp.txt" };
constexpr static unsigned samplingPeriod { 1 };

class BusTest : public testing::Test
{
protected:
    BusTest() noexcept = default;

    constexpr static const LinkedList<Bus>* generateInput(const char* const numOfBuses, const char* const beginTime,
                                                          const char* const endTime) noexcept
    {
        const char* const argv1[] = { nullptr, fileName, numOfBuses, beginTime, endTime };
        BusGenerator* bg { BusGenerator::create(5, argv1, samplingPeriod) };
        if (bg == nullptr)
        {
            return nullptr;
        }
        bg->printArrivals();
        return bg->getPeriods();
    }

    constexpr static const LinkedList<Bus>* analyzeInput() noexcept
    {
        constexpr const char* const argv2[] = { nullptr, fileName };
        BusAnalyzer* ba { BusAnalyzer::create(2, argv2, samplingPeriod) };
        if (ba == nullptr)
        {
            return nullptr;
        }
        ba->getSamples();
        ba->parseSamples();
        ba->extractPeriods();
        return ba->getPeriods();
    }

    Bus b1;
    Bus b2 { 5 };
    Bus b3 { 3, 8 };
    const LinkedList<Bus>* ll1 { nullptr };
    const LinkedList<Bus>* ll2 { nullptr };
};
