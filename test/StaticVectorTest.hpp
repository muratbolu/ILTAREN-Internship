#pragma once

#include <gtest/gtest.h>

#include "StaticVector.tpp"

class StaticVectorTest : public testing::Test
{
protected:
    StaticVectorTest()
    {
        vec1[0] = 3;
        vec1[1] = 5;
        vec1[2] = 7;

        vec2[0][0] = 12;
        vec2[4][3] = 15;
    }
    StaticVector<char, 2> vec0; // uninitialized
    StaticVector<int, 3> vec1;
    StaticVector<StaticVector<unsigned, 4>, 5> vec2;
};
