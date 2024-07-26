#pragma once

#include "exec/BusAnalyzer.tpp"
#include "exec/BusGenerator.tpp"
#include "sds/StaticStack.tpp"
#include "util/Bus.tpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <optional>

constexpr static const char* const fileName { "./temp.txt" };
constexpr static unsigned samplingPeriod { 1 };

#ifndef BUS_POOL_SIZE
#define BUS_POOL_SIZE 500
#endif

class BusTest : public testing::Test
{
protected:
    using BusStack = StaticStack<Bus, BUS_POOL_SIZE>;

    BusTest() noexcept = default;

    [[nodiscard]] static std::optional<BusGenerator> getGenerator(const char* const numOfBuses, const char* const beginTime,
                                                                  const char* const endTime) noexcept
    {
        const char* const argv[] = { nullptr, fileName, numOfBuses, beginTime, endTime };
        return BusGenerator::create(5, argv, samplingPeriod);
    }

    [[nodiscard]] static BusStack& getInput(BusGenerator& bg) noexcept
    {
        bg.printArrivals();
        bg.printSchedule();
        fputc('\n', stdout);
        return bg.getPeriods();
    }

    [[nodiscard]] static std::optional<BusAnalyzer> getAnalyzer() noexcept
    {
        constexpr const char* const argv[] = { nullptr, fileName };
        return BusAnalyzer::create(2, argv, samplingPeriod);
    }

    [[nodiscard]] static BusStack& getAnalysis(BusAnalyzer& ba) noexcept
    {
        ba.getSamples();
        ba.parseSamples();
        ba.extractPeriods();
        return ba.getPeriods();
    }

    Bus b1;
    Bus b2 { 5 };
    Bus b3 { 3, 8 };
};
