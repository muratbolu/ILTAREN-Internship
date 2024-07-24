#pragma once

#include "Bus.tpp"
#include "BusAnalyzer.tpp"
#include "BusGenerator.tpp"

#include <gtest/gtest.h>

class BusTest : public testing::Test
{
protected:
    BusTest() noexcept = default;
};
