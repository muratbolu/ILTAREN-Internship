#pragma once

#include "Bus.tpp"
#include "BusAnalyzer.tpp"
#include "BusGenerator.tpp"

#include <gtest/gtest.h>

class BusTest : public testing::Test
{
protected:
    BusTest() noexcept = default;
    Bus b1;
    Bus b2 { 5 };
    Bus b3 { 3, 8 };
};
