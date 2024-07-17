#pragma once

#include "StaticStack.tpp"
#include "StaticVector.tpp"

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>

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

        mNulls.fill(UINT_MAX);
        mBestMat.fill(mNulls);
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
                    mAdjacencyMatrix[line][sc - 3] = number;
                    number = 0;
                }
                else if (c == '\n')
                {
                    ++sc;
                    mAdjacencyMatrix[line][sc - 3] = number;
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

    constexpr bool matIsSubsetOf(const StaticVector<StaticVector<unsigned, 81>, 81>& fst,
                                 const StaticVector<StaticVector<unsigned, 81>, 81>& snd) const noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            for (unsigned j { 0 }; j < 81; ++j)
            {
                if (fst[i][j] != UINT_MAX && snd[i][j] == UINT_MAX)
                {
                    return false;
                }
            }
        }
        return true;
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

        mCurrStack.clear();
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
                // printf("mBestState.citiesStack.size(): %d\n", mBestState.citiesStack.size());
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
        mDFSStack.clear();
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
            if (mFilteredAdjacencyMatrix[cs[i]][cs[i + 1]] == UINT_MAX)
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
        // fprintf(stream, "Max mCurrStack usage: %d\n", mCurrStack.mMaxIndex);
    }

    static void printMatrixInfo(const StaticVector<StaticVector<unsigned, 81>, 81>& mat) noexcept
    {
        std::array<unsigned, 81 * 81> flattened;
        for (unsigned i { 0 }; i < 81; ++i)
        {
            for (unsigned j { 0 }; j < 81; ++j)
            {
                flattened[81 * i + j] = mat[i][j];
            }
        }
        unsigned sum { 0 };
        for (auto&& i : flattened)
        {
            sum += i;
        }
        printf("Sum: %d\n", sum);
        unsigned mean { sum / (81 * 81) };
        printf("Mean: %d\n", mean);

        /*
        puts("[");
        for (unsigned i { 0 }; i < 81 * 81; ++i)
        {
            if (i > 0)
            {
                printf(", ");
            }
            printf("%d", flattened[i]);
        }
        puts("]\n");
        */
        unsigned max { 0 };
        for (auto&& i : flattened)
        {
            if (i > max)
            {
                max = i;
            }
        }
        printf("Max: %d\n", max);

        unsigned min { UINT_MAX };
        for (auto&& i : flattened)
        {
            if (i < min)
            {
                min = i;
            }
        }
        printf("Min: %d\n", min);

        std::sort(flattened.begin(), flattened.end());
        unsigned median { flattened[(81 * 81) / 2 + 1] };
        printf("Median: %d\n", median);
    }

    constexpr static unsigned nthSmallest(const StaticVector<unsigned, 81 * 81>& lst, unsigned n) noexcept
    {
        unsigned index { select(lst, 0, 81 * 81 - 1, n - 1) };
        return lst[index];
    }

    constexpr static unsigned select(const StaticVector<unsigned, 81 * 81>& lst, unsigned left, unsigned right, unsigned n) noexcept
    {
        for (;;)
        {
            if (left == right)
            {
                return left;
            }
            unsigned pivotIndex { pivot(lst, left, right) };
            pivotIndex = partition(lst, left, right, pivotIndex, n);
            if (n == pivotIndex)
            {
                return n;
            }
            else if (n < pivotIndex)
            {
                right = pivotIndex - 1;
            }
            else
            {
                left = pivotIndex + 1;
            }
        }
    }

    constexpr static unsigned pivot(StaticVector<unsigned, 81 * 81> lst, unsigned left, unsigned right) noexcept
    {
        if (right - left < 5)
        {
            return partition5(lst, left, right);
        }
        for (unsigned i { left }; i <= right; i += 5)
        {
            unsigned subRight { i + 4 };
            if (subRight > right)
            {
                subRight = right;
            }
            unsigned median5 { partition5(lst, i, subRight) };
            std::swap(lst[median5], lst[left + static_cast<unsigned>(std::floor((i - left) / 5))]);
        }
        unsigned mid { static_cast<unsigned>(std::floor((right - left) / 10)) + left + 1 };
        return select(lst, left, left + static_cast<unsigned>(std::floor((right - left) / 5)), mid);
    }

    constexpr static unsigned partition(StaticVector<unsigned, 81 * 81> lst, unsigned left, unsigned right, unsigned pivotIndex, unsigned n) noexcept
    {
        unsigned pivotValue { lst[pivotIndex] };
        std::swap(lst[pivotIndex], lst[right]);
        unsigned storeIndex { left };
        for (unsigned i { left }; i < right; ++i)
        {
            if (lst[i] < pivotValue)
            {
                std::swap(lst[storeIndex++], lst[i]);
            }
        }
        unsigned storeIndexEq { storeIndex };
        for (unsigned i { storeIndex }; i < right; ++i)
        {
            if (lst[i] == pivotValue)
            {
                std::swap(lst[storeIndexEq++], lst[i]);
            }
        }
        std::swap(lst[right], lst[storeIndexEq]);
        if (n < storeIndex)
        {
            return storeIndex;
        }
        if (n <= storeIndexEq)
        {
            return n;
        }
        return storeIndexEq;
    }

    constexpr static unsigned partition5(StaticVector<unsigned, 81 * 81> lst, unsigned left, unsigned right) noexcept
    {
        unsigned i { left + 1 };
        while (i <= right)
        {
            unsigned j { i };
            while (j > left && lst[j - 1] > lst[j])
            {
                std::swap(lst[j - 1], lst[j]);
                j = j - 1;
            }
            i = i + 1;
        }
        return left + (right - left) / 2;
    }

    // Filled in by parseInput()
    static inline StaticVector<StaticVector<unsigned, 81>, 81> mAdjacencyMatrix;
    // Filled in by filterByRange()
    static inline StaticVector<StaticVector<unsigned, 81>, 81> mFilteredAdjacencyMatrix;

    // For mBestMat initialization
    static inline StaticVector<unsigned, 81> mNulls;
    static inline StaticVector<StaticVector<unsigned, 81>, 81> mBestMat;
private:
    // Used by printRoute. Must be called after parseInput()
    constexpr static StaticVector<char, MAX_NAME_SIZE> toNames(unsigned n) noexcept
    {
        return Traveler::mCityNames[n];
    }

    [[nodiscard]] constexpr bool traversable(unsigned n, unsigned m) const noexcept
    {
        return mFilteredAdjacencyMatrix[n][m] < UINT_MAX;
    }

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
    // Filled in by constructor
    unsigned mStartCity, x, y;

    // Filled in by travel method
    static inline State mBestState;
};
