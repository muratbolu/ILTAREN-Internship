#pragma once

#include "io/IO.tpp"
#include "sds/StaticStack.tpp"
#include "util/Bus.tpp"
#include "util/Timer.tpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <optional>

#define MAX_LINE_LEN 64

#ifndef BUS_POOL_SIZE
#define BUS_POOL_SIZE 500
#endif

#define MAX_SAMPLES 2000

class BusAnalyzer
{
    using Time = timer::Time;
    using Dur = timer::Duration;
public:
    constexpr BusAnalyzer() noexcept = delete;
    constexpr ~BusAnalyzer() noexcept = default;
    constexpr BusAnalyzer(const BusAnalyzer&) noexcept = delete;
    constexpr BusAnalyzer(BusAnalyzer&&) noexcept = default;
    constexpr BusAnalyzer& operator=(const BusAnalyzer&) noexcept = delete;
    constexpr BusAnalyzer& operator=(BusAnalyzer&&) noexcept = default;

    constexpr static std::optional<BusAnalyzer> create(int argc, const char* const argv[], unsigned samplingPeriod) noexcept
    {
        if (argc != 2)
        {
            int rc = fputs("Provide exactly one argument: input file.\n", stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
            return std::nullopt;
        }
        FILE* fr = fopen(argv[1], "r");   // NOLINT
        if (fr == nullptr)
        {
            perror("Could not open file");
            return std::nullopt;
        }
        // Input from fr, output to stdout
        return std::make_optional<BusAnalyzer>({ fr, stdout, samplingPeriod });
    }

    void getSamples() noexcept
    {
        // Assume that EOF is not zero in the implementation
        for (;;)
        {
            mLines.pushBack(StaticStack<char, MAX_LINE_LEN> {});
            StaticStack<char, MAX_LINE_LEN>& back { mLines.back() };
            int c { EOF };
            for (;;)
            {
                c = fgetc(istream);
                if (c == EOF || c == '\n')
                {
                    back.pushBack('\0');
                    break;
                }
                back.pushBack(static_cast<char>(c));
            }
            if (c == EOF)
            {
                mLines.popBack();
                break;
            }
        }
    }

    void parseSamples() noexcept
    {
        assert(strlen(mLines.front().data()) == 5);
        Time begin { mLines.front().data() };
        assert(strlen(mLines.back().data()) > 6);
        Time end { mLines.back().data() };
        for (Dur d { mSP }; begin + d <= end; d += mSP)
        {
            mSamples.pushBack(0);
        }
        for (auto&& l : mLines)
        {
            if (strlen(l.data()) < 7)
            {
                continue;
            }
            mSamples[(Time { l.data() } - begin).getDuration() / mSP] = atoi(l.data() + 6);
        }
        // io::print(ostream, mSamples);
        // fputc('\n', ostream);
    }

    void extractPeriods() noexcept
    {
        StaticStack<unsigned, MAX_SAMPLES> samples { mSamples };
        for (;;)
        {
            // t.first := offset, t.second = period
            Bus b { extractPeriod(samples) };
            if (!b.isValid())
            {
                break;
            }
            mBuses.pushBack(b * mSP);
            for (unsigned i { b.getFirst() }; i < samples.size(); i += b.getFirst())
            {
                if (i == 0)
                {
                    continue;
                }
                assert(samples[i] > 0);
                --samples[i];
                i += b.getSecond();
                if (i < samples.size())
                {
                    if (samples[i] == 0)
                    {
                        io::print(stdout, samples);
                        fputc('\n', stdout);
                        io::print(stdout, i);
                        fputc('\n', stdout);
                        io::print(stdout, b);
                        fputc('\n', stdout);
                        io::print(stdout, mBuses);
                        fputc('\n', stdout);
                        assert(0 && "samples[i] > 0 failed");
                    }
                    --samples[i];
                }
            }
        }
    }

    [[nodiscard]] constexpr static StaticStack<unsigned, MAX_SAMPLES> shiftIndex(const StaticStack<unsigned, MAX_SAMPLES>& s, unsigned n) noexcept
    {
        StaticStack<unsigned, MAX_SAMPLES> shifted;
        for (unsigned i { n }; i < s.size(); ++i)
        {
            shifted.pushBack(s[i]);
        }
        return shifted;
    }

    [[nodiscard]] static Bus extractPeriod(const StaticStack<unsigned, MAX_SAMPLES>& s) noexcept
    {
        // i := period
        // j := offset
        for (unsigned i { 1 }; i <= s.size() / 2; ++i)
        {
            for (unsigned j { 1 }; j <= i; ++j)
            {
                if (s[j] > 0)
                {
                    bool isValid { true };
                    for (unsigned k { j }; k < s.size(); k += j)
                    {
                        if (s[k] == 0)
                        {
                            isValid = false;
                            break;
                        }
                        k += i - j;
                        if (k < s.size() && s[k] == 0)
                        {
                            isValid = false;
                            break;
                        }
                    }
                    if (isValid)
                    {
                        // TODO: refactor
                        if (j == i)
                        {
                            return Bus { i };
                        }
                        assert(i >= j);
                        assert(i - j != j);
                        return Bus { j, i - j };
                    }
                }
            }
        }
        return Bus { 0 };
    }

    [[nodiscard]] constexpr StaticStack<Bus, BUS_POOL_SIZE>& getPeriods() noexcept
    {
        return mBuses;
    }

    void printResult() const noexcept
    {
        fprintf(ostream, "Number of buses found: %d\n", mBuses.size());
        io::print(ostream, mBuses);
        fputc('\n', ostream);
    }
private:
    // Input and output files
    FILE *istream, *ostream;
    // Sampling period
    unsigned mSP;
    // Input lines
    StaticStack<StaticStack<char, MAX_LINE_LEN>, BUS_POOL_SIZE> mLines;
    // Samples gathered from parsing input file
    StaticStack<unsigned, MAX_SAMPLES> mSamples;
    // Buses found by extracting periods
    StaticStack<Bus, BUS_POOL_SIZE> mBuses;

    // Private constructor
    constexpr BusAnalyzer(FILE* is, FILE* os, unsigned samplingPeriod) noexcept :
        istream { is },
        ostream { os },
        mSP { samplingPeriod }
    {
    }
};
