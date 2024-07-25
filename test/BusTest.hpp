#pragma once

#include "util/Bus.tpp"

#include <gtest/gtest.h>

class BusTest : public testing::Test
{
protected:
    BusTest() noexcept = default;
    Bus b1;
    Bus b2 { 5 };
    Bus b3 { 3, 8 };
};
