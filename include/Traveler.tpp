#pragma once

#include "StaticStack.tpp"
#include "StaticVector.tpp"

#include <cassert>
#include <climits>
#include <cstdio>
#include <cstdlib>

// Separator is semicolon for our file
#define SEP ';'

// 2^16 char size buffer
#define BUF_SIZE (1024 * 64)

// max 32 characters for city names
#define MAX_NAME_SIZE (32)

class Traveler
{
    struct State
    {
        StaticStack<unsigned, 81> citiesStack;
        StaticVector<bool, 81> visitedArray;
        unsigned visitedCount;

        constexpr State() noexcept = default;

        // try to delete this
        // constexpr State(const State&) noexcept = delete;
        constexpr State(const State&) noexcept = default;

        constexpr State(const StaticStack<unsigned, 81>& citiesStack, const StaticVector<bool, 81>& visitedArray, const unsigned& visitedCount) :
            citiesStack { citiesStack },
            visitedArray { visitedArray },
            visitedCount { visitedCount }
        {
        }

        constexpr State& operator=(const State&) noexcept = default;
        constexpr State(State&&) noexcept = default;
        constexpr State& operator=(State&&) noexcept = default;

        constexpr inline bool operator==(const State&) const noexcept = default;
    };
public:
    Traveler(char arg1[], char arg2[], char arg3[], char arg4[]) noexcept :
        mStartCity { static_cast<unsigned>(atoi(arg2) - 1) },
        x { static_cast<unsigned>(atoi(arg3)) },
        y { static_cast<unsigned>(atoi(arg4)) }
    {
        getInput(arg1);
        parseInput();
    }

    // returns false if something goes wrong but we don't use it
    static bool getInput(char arg[]) noexcept
    {
        FILE* fp = fopen(arg, "r");
        if (fp == nullptr)
        {
            perror("Could not open file");
            return false;
        }

        for (int c { EOF }; char& i : mBuffer)
        {
            c = fgetc(fp);
            if (c == EOF)
            {
                break;
            }
            i = static_cast<char>(c);
        }

        if (ferror(fp))
        {
            perror("I/O error while reading file");
            fclose(fp);
            return false;
        }
        if (feof(fp))
        {
            /*
            puts("Successfully read file: ");
            puts(arg);
            */
            fclose(fp);
            return true;
        }
        puts("No error and didn't reach EOF");
        fclose(fp);
        return false;
    }

    // Assumes that the input is well-formed.
    void parseInput() noexcept
    {
        // sc keeps tracks of how many semicolons are encountered
        unsigned sc { 0 };
        // line keeps track of the line number
        unsigned line { 0 };
        // i keeps track of the character index in city names
        unsigned i { 0 };
        // number calculates the number for each semicolon separated
        // value
        unsigned number { 0 };

        enum parseStates
        {
            SKIP_CODE,
            GET_NAME,
            CALC_NUM,
        } state { SKIP_CODE };

        for (char c : mBuffer)
        {
            if (c == '\0')
            {
                break;
            }

            switch (state)
            {
            case SKIP_CODE : {
                if (c == SEP)
                {
                    ++sc;
                    i = 0;
                    state = GET_NAME;
                }
                break;
            }
            case GET_NAME : {
                if (c == SEP)
                {
                    Traveler::mCityNames[line][i++] = '\0';
                    ++sc;
                    state = CALC_NUM;
                }
                else
                {
                    Traveler::mCityNames[line][i++] = c;
                }
                break;
            }
            case CALC_NUM : {
                if (c == SEP)
                {
                    ++sc;
                    adjacencyMatrix[line][sc - 3] = number;
                    number = 0;
                }
                else if (c == '\n')
                {
                    ++sc;
                    adjacencyMatrix[line][sc - 3] = number;
                    sc = 0;
                    number = 0;
                    ++line;
                    state = SKIP_CODE;
                }
                else
                {
                    number *= 10;
                    number += static_cast<unsigned>(c - '0');
                }
                break;
            }
            }
        }
    }

    constexpr static void printMatrix(FILE* stream, const StaticVector<StaticVector<unsigned, 81>, 81>& mat) noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            fprintf(stream, "%02d", i + 1);
            fputc(SEP, stream);
            fprintf(stream, "%s", Traveler::mCityNames[i].data());
            fputc(SEP, stream);
            for (unsigned j { 0 }; j < 81; ++j)
            {
                fprintf(stream, "%d", mat[i][j]);
                if (j != 80)
                {
                    fputc(SEP, stream);
                }
            }
            fprintf(stream, "\n");
        }
    }

    constexpr void filterByRange(StaticVector<StaticVector<unsigned, 81>, 81>& dst,
                                 const StaticVector<StaticVector<unsigned, 81>, 81>& src) const noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            for (unsigned j { 0 }; j < 81; ++j)
            {
                if ((src[i][j] < (x - y)) || (src[i][j] > (x + y)))
                {
                    dst[i][j] = UINT_MAX;
                }
                else
                {
                    dst[i][j] = src[i][j];
                }
            }
        }
    }

    // Zero-indexing for cities, 0 => ADANA, etc.
    void travel() noexcept
    {
        visitableCities(mStartCity);   // writes to mBest
    }

    /* Tries to find the longest path by brute force, caching previously
     * visited configurations in mVisitedStack.
     */
    void visitableCities(unsigned n) noexcept
    {
        mBestState.citiesStack.clear();
        mBestState.citiesStack.pushBack(n);

        mBestState.visitedArray.fill(false);
        mBestState.visitedArray[n] = true;

        mBestState.visitedCount = 1;

        mCurrStack.pushBack(mBestState);

        mBestState.citiesStack.clear();
        mBestState.visitedArray.fill(false);

        unsigned notChanged { 0 };
        while (notChanged < 10000 && !mCurrStack.empty())
        {
            const State& currState { mCurrStack.popBack() };

            if (isSubsetOf(currState.visitedArray, mBestState.visitedArray))
            {
                notChanged++;
                continue;
            }

            if (currState.visitedCount + dfs(currState) < mBestState.visitedCount)
            {
                notChanged++;
                continue;
            }

            if (currState.visitedCount > mBestState.visitedCount)
            {
                notChanged = 0;
                mBestState = currState;
                printf("mBestState.citiesStack.size(): %d\n", mBestState.citiesStack.size());
            }

            /* Caching visited sets do not work
            if (stackContains(mVisitedSet, state))
            {
                continue;
            }
            mVisitedSet.pushBack(state);
            */

            // StaticStack<unsigned, 81> validCities;
            mValidCities.clear();
            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (!currState.visitedArray[i] && traversable(currState.citiesStack.back(), i))
                {
                    mValidCities.pushBack(i);
                }
            }
            // StaticStack<unsigned, 81> newCities { cities };
            mNewCities = currState.citiesStack;
            // StaticVector<bool, 81> newVisited { visited };
            mNewVisited = currState.visitedArray;
            for (const unsigned& c : mValidCities)
            {
                mNewCities.pushBack(c);
                mNewVisited[c] = true;
                mCurrStack.pushBack({ mNewCities, mNewVisited, mNewVisited.count(true) });
                mNewCities.popBack();
                mNewVisited[c] = false;
            }
        }
    }

    /*
    template<unsigned U>
    [[nodiscard]] constexpr bool stackContains(const StaticStack<State, U>& stack, const State& s) const noexcept
    {
        for (unsigned i { 0 }; i < stack.size(); ++i)
        {
            if (stack[i].currCity == s.currCity && isSubsetOf(stack[i].visitedArray, s.visitedArray))
            {
                return true;
            }
        }
        return false;
    }
    */

    [[nodiscard]] constexpr static bool isSubsetOf(const StaticVector<bool, 81>& fst, const StaticVector<bool, 81>& snd) noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (fst[i] && !snd[i])
            {
                return false;
            }
        }
        return true;
    }

    // Return the number of reachable states from a starting state.
    [[nodiscard]] unsigned dfs(const State& s) const noexcept
    {
        unsigned reachable { 0 };

        // StaticVector<bool, 81> reachables;
        mReachables.fill(false);

        // StaticVector<unsigned, 81> stack;
        mDFSStack.pushBack(s.citiesStack.back());

        while (mDFSStack.size() > 0)
        {
            unsigned curr = mDFSStack.popBack();
            if (mReachables[curr])
            {
                continue;
            }
            mReachables[curr] = true;
            reachable++;
            for (unsigned i = 0; i < 81; ++i)
            {
                if (traversable(curr, i) && !s.visitedArray[i] && !mReachables[i] && !mDFSStack.contains(i))
                {
                    mDFSStack.pushBack(i);
                }
            }
        }

        // We delete the reachability of the start node and consider only
        // the "movable" states.
        mReachables[s.citiesStack.back()] = false;

        return reachable;
    }

    [[nodiscard]] constexpr bool validator(const StaticStack<unsigned, 81>& cs) const noexcept
    {
        for (unsigned i { 0 }; i + 1 < cs.size(); ++i)
        {
            if (filteredAdjacencyMatrix[cs[i]][cs[i + 1]] == UINT_MAX)
            {
                return false;
            }
        }
        for (unsigned i { 0 }; i < cs.size(); ++i)
        {
            for (unsigned j { i + 1 }; j < cs.size(); ++j)
            {
                if (cs[i] == cs[j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    [[nodiscard]] constexpr static unsigned countValid(const StaticVector<unsigned, 81>& v) noexcept
    {
        return v.size() - v.count(UINT_MAX);
    }

    void printRoute(FILE* stream) const noexcept
    {
        fprintf(stream, "Length: %d\n", countValid(mBestState.citiesStack));
        fputs("[", stream);
        for (unsigned i { 0 }; i < mBestState.citiesStack.size(); ++i)
        {
            if (i > 0)
            {
                fputs(", ", stream);
            }
            fprintf(stream, "%s", static_cast<char*>(toNames(mBestState.citiesStack[i]).data()));
        }
        fputs("]\n", stream);
        fprintf(stream, "Max mCurrStack usage: %d\n", mCurrStack.mMaxIndex);
    }

    // Filled in by parseInput()
    StaticVector<StaticVector<unsigned, 81>, 81> adjacencyMatrix;
    // Filled in by filterByRange()
    StaticVector<StaticVector<unsigned, 81>, 81> filteredAdjacencyMatrix;
private:
    // Used by printRoute. Must be called after parseInput()
    constexpr static StaticVector<char, MAX_NAME_SIZE> toNames(unsigned n) noexcept
    {
        return Traveler::mCityNames[n];
    }

    [[nodiscard]] constexpr bool traversable(unsigned n, unsigned m) const noexcept
    {
        return filteredAdjacencyMatrix[n][m] < UINT_MAX;
    }

    // Filled in by constructor
    unsigned mStartCity, x, y;

    // Used by parseInput()
    static inline StaticVector<char, BUF_SIZE> mBuffer;

    // Filled in by parseInput()
    static inline StaticVector<StaticVector<char, MAX_NAME_SIZE>, 81> mCityNames;

    // For visitableCities
    static inline StaticStack<State, 50000> mCurrStack;   // state stack
    static inline StaticStack<unsigned, 81> mValidCities;
    static inline StaticStack<unsigned, 81> mNewCities;
    static inline StaticVector<bool, 81> mNewVisited;

    // For DFS
    static inline StaticVector<bool, 81> mReachables;
    static inline StaticStack<unsigned, 81> mDFSStack;
public:
    static inline State mBestState;
};
