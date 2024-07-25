#pragma once

#include "iter/ContIter.tpp"

#include <gtest/gtest.h>

class ContIterTest : public testing::Test
{
protected:
    ContIterTest()
    {
        *it1 = 5;
    }

    int p1;
    ContIter<int> it1 { &p1 };
    ContIter<int> it2 { &p1 };

    int p2[4];
    ContIter<int> it3 { p2 };
};
