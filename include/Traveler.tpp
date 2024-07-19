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
public:
    struct State
    {
        StaticStack<unsigned, 81> citiesStack;
        StaticVector<bool, 81> visitedArray;
        unsigned visitedCount;

        constexpr State() noexcept :
            visitedCount { 0 }
        {
        }

        constexpr State(const StaticStack<unsigned, 81>& cs, const StaticVector<bool, 81>& va, const unsigned& vc) noexcept :
            citiesStack { cs },
            visitedArray { va },
            visitedCount { vc }
        {
        }

        constexpr ~State() noexcept = default;

        // try to delete this
        // constexpr State(const State&) noexcept = delete;
        constexpr State(const State&) noexcept = default;
        constexpr State(State&&) noexcept = default;
        // try to delete this
        // constexpr State& operator=(const State&) noexcept = delete;
        constexpr State& operator=(const State&) noexcept = default;
        constexpr State& operator=(State&&) noexcept = default;

        constexpr inline bool operator==(const State&) const noexcept = default;
    };

    constexpr Traveler() noexcept = default;
    constexpr ~Traveler() noexcept = default;
    constexpr Traveler(const Traveler&) noexcept = delete;
    constexpr Traveler(Traveler&&) noexcept = delete;
    constexpr Traveler& operator=(const Traveler&) noexcept = delete;
    constexpr Traveler& operator=(Traveler&&) noexcept = delete;

    Traveler(int argc, char* argv[]) noexcept
    {
        if (argc != 5)
        {
            int rc = fputs("Provide exactly four arguments: input file, start city "
                           "code, X, and Y.\n",
                           stderr);
            if (rc == EOF)
            {
                perror("Invalid argument number");
            }
        }
        else
        {
            mStartCity = static_cast<unsigned>(atoi(argv[2]) - 1);
            mX = static_cast<unsigned>(atoi(argv[3]));
            mY = static_cast<unsigned>(atoi(argv[4]));
            getInput(argv[1]);
            parseInput();
            searchBestPath();
        }
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
    static void parseInput() noexcept
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
                    mCityNames[line][i++] = '\0';
                    ++sc;
                    state = CALC_NUM;
                }
                else
                {
                    mCityNames[line][i++] = c;
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
            fprintf(stream, "%s", mCityNames[i].data());
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

    static void filterByRange(StaticVector<StaticVector<unsigned, 81>, 81>& dst, const StaticVector<StaticVector<unsigned, 81>, 81>& src) noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            for (unsigned j { 0 }; j < 81; ++j)
            {
                if ((src[i][j] < (mX - mY)) || (src[i][j] > (mX + mY)))
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

    [[nodiscard]] constexpr static bool stackContains(const StaticVector<StaticVector<unsigned, 81>, 81>& fst,
                                                      const StaticStack<StaticVector<StaticVector<unsigned, 81>, 81>, 81>& stack) noexcept
    {
        for (unsigned i { stack.size() }; i-- > 0;)
        {
            if (matIsSubsetOf(fst, stack[i]))
            {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] constexpr static bool matIsSubsetOf(const StaticVector<StaticVector<unsigned, 81>, 81>& fst,
                                                      const StaticVector<StaticVector<unsigned, 81>, 81>& snd) noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            for (unsigned j { i + 1 }; j < 81; ++j)
            {
                if (fst[i][j] != UINT_MAX && snd[i][j] == UINT_MAX)
                {
                    return false;
                }
            }
        }
        return true;
    }

    static void searchBestPath() noexcept
    {
#define I (i)
#define J (j)
#define Y (J)               // j = y
#define FUNC(x) (2 * (x))   // f(x) = x^2
#define X (I - FUNC(Y))     // i = x + f(y)
#define LIM 5000

        static StaticStack<unsigned int, 81> cs;
        cs.pushBack(Traveler::mStartCity);
        static StaticVector<bool, 81> va;
        va[Traveler::mStartCity] = true;
        static unsigned vc { va.count(true) };
        static Traveler::State startState { cs, va, vc };

        for (int i { 1 }; i < LIM; ++i)
        {
            for (int j { 1 }; Y <= X; ++j)   // t.x >= t.y
            {
                assert(X > 0);
                assert(Y > 0);
                Traveler::mX = static_cast<unsigned>(X);
                Traveler::mY = static_cast<unsigned>(Y);
                // printf("%d:  t.x: %d, t.y: %d\n", i, t.x, t.y);
                Traveler::filterByRange(Traveler::mFilteredAdjacencyMatrix, Traveler::mAdjacencyMatrix);
                if (Traveler::stackContains(Traveler::mFilteredAdjacencyMatrix, Traveler::mBestMats))
                {
                    continue;
                }
                if (Traveler::dfs(startState) < 81)
                {
                    continue;
                }
                Traveler::travel();
                if (Traveler::validator(Traveler::mBestState.citiesStack))
                {
                    static unsigned currMax { 0 };
                    if (Traveler::mBestState.visitedCount > currMax)
                    {
                        Traveler::printRoute(stdout);
                        currMax = Traveler::mBestState.visitedCount;
                        Traveler::mBestMats.pushBack(Traveler::mFilteredAdjacencyMatrix);
                        printf("x: %d, y: %d\n\n", Traveler::mX, Traveler::mY);
                        if (currMax == 81)
                        {
                            return;
                        }
                    }
                }
                else
                {
                    puts("Invalid route");
                }
            }
        }
    }

    // Zero-indexing for cities, 0 => ADANA, etc.
    static void travel() noexcept
    {
        visitableCities(mStartCity);   // writes to mBest
    }

    /* Tries to find the longest path by brute force, caching previously
     * visited configurations in mVisitedStack.
     */
    static void visitableCities(unsigned n) noexcept
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
    [[nodiscard]] static unsigned dfs(const State& s) noexcept
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

    [[nodiscard]] constexpr static bool validator(const StaticStack<unsigned, 81>& cs) noexcept
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

    static void printRoute(FILE* stream) noexcept
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

    // Filled in by parseInput()
    static inline StaticVector<StaticVector<unsigned, 81>, 81> mAdjacencyMatrix;
    // Filled in by filterByRange()
    static inline StaticVector<StaticVector<unsigned, 81>, 81> mFilteredAdjacencyMatrix;

    // For mBestMat initialization
    static inline StaticVector<unsigned, 81> mNulls;
    static inline StaticStack<StaticVector<StaticVector<unsigned, 81>, 81>, 81> mBestMats;
private:
    // Used by printRoute. Must be called after parseInput()
    constexpr static StaticVector<char, MAX_NAME_SIZE> toNames(unsigned n) noexcept
    {
        return mCityNames[n];
    }

    [[nodiscard]] constexpr static bool traversable(unsigned n, unsigned m) noexcept
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
    static inline unsigned mStartCity, mX, mY;

    // Filled in by travel method
    static inline State mBestState;
};
