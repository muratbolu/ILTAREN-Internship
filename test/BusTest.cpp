#include "BusTest.hpp"

#include "exec/BusAnalyzer.tpp"
#include "exec/BusGenerator.tpp"
#include "io/IO.tpp"
#include "sds/LinkedList.tpp"
#include "sds/StaticStack.tpp"
#include "util/Bus.tpp"

#include <cstdio>
#include <gtest/gtest.h>
#include <optional>

TEST_F(BusTest, Initialization)
{
}

TEST_F(BusTest, Get)
{
    EXPECT_EQ(b1.getFirst(), 0);
    EXPECT_EQ(b1.getSecond(), 0);
    EXPECT_EQ(b2.getFirst(), 5);
    EXPECT_EQ(b2.getSecond(), 5);
    EXPECT_EQ(b3.getFirst(), 3);
    EXPECT_EQ(b3.getSecond(), 8);
}

TEST_F(BusTest, Alternating)
{
    EXPECT_TRUE(!b1.isAlternating());
    EXPECT_TRUE(!b2.isAlternating());
    EXPECT_TRUE(b3.isAlternating());
}

TEST_F(BusTest, Valid)
{
    EXPECT_TRUE(!b1.isValid());
    EXPECT_TRUE(b2.isValid());
    EXPECT_TRUE(b3.isValid());
}

TEST_F(BusTest, Multiply)
{
    b2 *= 3;
    EXPECT_EQ(b2.getFirst(), 15);
    EXPECT_EQ(b2.getSecond(), 15);

    EXPECT_EQ((b3 * 2).getFirst(), 6);
    EXPECT_EQ((b3 * 2).getSecond(), 16);
}

TEST_F(BusTest, Sum)
{
    EXPECT_EQ(b2.sum(), 10);
    EXPECT_EQ(b3.sum(), 11);
}

TEST_F(BusTest, Equality)
{
    EXPECT_TRUE(b1 == Bus {});
    EXPECT_TRUE(b1 == Bus { 0 });
    EXPECT_TRUE(b2 == Bus { 5 });
    EXPECT_TRUE((b3 == Bus { 3, 8 }));

    EXPECT_FALSE(b1 == Bus { 5 });
    EXPECT_FALSE((b1 == Bus { 3, 8 }));
}

TEST_F(BusTest, OneBusSixHourCase)
{
    std::optional<BusGenerator> bg { getGenerator("1", "12:00", "18:00") };
    ASSERT_TRUE(bg.has_value());
    BusStack ll1 { getInput(bg.value()) };

    std::optional<BusAnalyzer> ba { getAnalyzer() };
    ASSERT_TRUE(ba.has_value());
    BusStack ll2 { getAnalysis(ba.value()) };

    ll1.sort();
    ll2.sort();
    EXPECT_EQ(ll1, ll2);

    io::print(stdout, ll1);
    fputc('\n', stdout);
    io::print(stdout, ll2);
    fputc('\n', stdout);
}

TEST_F(BusTest, TwoBusesThreeHourCase)
{
    std::optional<BusGenerator> bg { getGenerator("2", "20:00", "23:00") };
    ASSERT_TRUE(bg.has_value());
    BusStack ll1 { getInput(bg.value()) };

    std::optional<BusAnalyzer> ba { getAnalyzer() };
    ASSERT_TRUE(ba.has_value());
    BusStack ll2 { getAnalysis(ba.value()) };

    ll1.sort();
    ll2.sort();
    EXPECT_EQ(ll1, ll2);

    io::print(stdout, ll1);
    fputc('\n', stdout);
    io::print(stdout, ll2);
    fputc('\n', stdout);
}

TEST_F(BusTest, TwoBusesOneHourCase)
{
    std::optional<BusGenerator> bg { getGenerator("2", "00:00", "01:00") };
    ASSERT_TRUE(bg.has_value());
    BusStack ll1 { getInput(bg.value()) };

    std::optional<BusAnalyzer> ba { getAnalyzer() };
    ASSERT_TRUE(ba.has_value());
    BusStack ll2 { getAnalysis(ba.value()) };

    ll1.sort();
    ll2.sort();
    EXPECT_EQ(ll1, ll2);

    io::print(stdout, ll1);
    fputc('\n', stdout);
    io::print(stdout, ll2);
    fputc('\n', stdout);
}

TEST_F(BusTest, FiveBusesFiveHourCase)
{
    std::optional<BusGenerator> bg { getGenerator("5", "10:00", "15:00") };
    ASSERT_TRUE(bg.has_value());
    BusStack ll1 { getInput(bg.value()) };

    std::optional<BusAnalyzer> ba { getAnalyzer() };
    ASSERT_TRUE(ba.has_value());
    BusStack ll2 { getAnalysis(ba.value()) };

    ll1.sort();
    ll2.sort();
    EXPECT_EQ(ll1, ll2);

    io::print(stdout, ll1);
    fputc('\n', stdout);
    io::print(stdout, ll2);
    fputc('\n', stdout);
}

TEST_F(BusTest, TenBusesTwoHourCase)
{
    std::optional<BusGenerator> bg { getGenerator("10", "11:00", "13:00") };
    ASSERT_TRUE(bg.has_value());
    BusStack ll1 { getInput(bg.value()) };

    std::optional<BusAnalyzer> ba { getAnalyzer() };
    ASSERT_TRUE(ba.has_value());
    BusStack ll2 { getAnalysis(ba.value()) };

    ll1.sort();
    ll2.sort();
    EXPECT_EQ(ll1, ll2);

    io::print(stdout, ll1);
    fputc('\n', stdout);
    io::print(stdout, ll2);
    fputc('\n', stdout);
}

TEST_F(BusTest, TwentyBusesOneHourCase)
{
    std::optional<BusGenerator> bg { getGenerator("20", "11:00", "12:00") };
    ASSERT_TRUE(bg.has_value());
    BusStack ll1 { getInput(bg.value()) };

    std::optional<BusAnalyzer> ba { getAnalyzer() };
    ASSERT_TRUE(ba.has_value());
    BusStack ll2 { getAnalysis(ba.value()) };

    ll1.sort();
    ll2.sort();
    EXPECT_EQ(ll1, ll2);

    io::print(stdout, ll1);
    fputc('\n', stdout);
    io::print(stdout, ll2);
    fputc('\n', stdout);
}
