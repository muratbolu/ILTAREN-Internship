#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticVector.tpp"

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
    Traveler(char arg1[], char arg2[], char arg3[], char arg4[]) noexcept :
        mStartCity { static_cast<unsigned>(atoi(arg2) - 1) },
        x { static_cast<unsigned>(atoi(arg3)) },
        y { static_cast<unsigned>(atoi(arg4)) }
    {
        mCitiesStack.pool() = &mLLPool;
        mCitiesStack.pushBack(LinkedList<unsigned> { &mNodePool });
        cities = mCitiesStack.back();

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
    constexpr void parseInput() noexcept
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
    constexpr void travel() noexcept
    {
        cities->pushBack(mStartCity);
        visitableCities(mStartCity);   // writes to cities
    }

    // TODO: Eliminate stack allocations

    /* Tries to find the longest path by picking the neighbor
     * with the highest DFS-Score, i.e., the node that can
     * reach the most nodes without visiting the previously
     * visited ones.
     */
    constexpr void visitableCities(unsigned n) const noexcept
    {
        unsigned currCity { n };
        StaticVector<bool, 81> visitedArr;
        visitedArr.clear();
        visitedArr[currCity] = true;

        bool notStuck { true };
        while (notStuck)
        {
            notStuck = false;
            unsigned nextCity { static_cast<unsigned>(-1) };
            int maxVisitable { -1 };
            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (!traversable(currCity, i) || visitedArr[i])
                {
                    continue;
                }
                int visitable { static_cast<int>(countTrue(dfs(visitedArr, i))) };
                if (maxVisitable < visitable)
                {
                    notStuck = true;
                    nextCity = i;
                    maxVisitable = visitable;
                }
            }
            if (notStuck)
            {
                currCity = nextCity;
                cities->pushBack(currCity);
                visitedArr[currCity] = true;
            }
        }
    }

    /* Makes a stack allocation. Return the reachable states, not including
     * those in the visitedArr, starting from n.
     */
    [[nodiscard]] constexpr StaticVector<bool, 81> dfs(const StaticVector<bool, 81>& visitedArr, const unsigned& n) const noexcept
    {
        StaticVector<bool, 81> reachables;
        reachables.clear();

        StaticVector<unsigned, 81> stack;
        unsigned currIndex { 0 };
        stack[currIndex++] = n;

        while (currIndex)
        {
            unsigned curr = stack[--currIndex];
            if (reachables[curr])
            {
                continue;
            }
            reachables[curr] = true;
            for (unsigned i = 0; i < 81; ++i)
            {
                if (traversable(curr, i) && !visitedArr[i] && !reachables[i] && !stack.contains(i))
                {
                    stack[currIndex++] = i;
                }
            }
        }

        /* We delete the reachability of the start node and consider only
         * the "movable" states.
         */
        reachables[n] = false;
        return reachables;
    }

    [[nodiscard]] constexpr static unsigned countTrue(const StaticVector<bool, 81>& v) noexcept
    {
        unsigned result { 0 };
        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (v[i])
            {
                ++result;
            }
        }
        return result;
    }

    [[nodiscard]] constexpr unsigned countEdges(const StaticVector<bool, 81>& visited, unsigned n) const noexcept
    {
        unsigned edges { 0 };
        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (traversable(n, i) && !visited[i])
            {
                ++edges;
            }
        }
        return edges;
    }

    [[nodiscard]] constexpr bool validator(const LinkedList<unsigned>& cs) const noexcept
    {
        for (unsigned i { 0 }; (i + 1) < cs.size(); ++i)
        {
            if (filteredAdjacencyMatrix[*cs[i]][*cs[i + 1]] == UINT_MAX)
            {
                return false;
            }
        }
        for (unsigned i { 0 }; i < cs.size(); ++i)
        {
            for (unsigned j { i + 1 }; j < cs.size(); ++j)
            {
                if (*cs[i] == *cs[j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    void printRoute(FILE* stream) const noexcept
    {
        fprintf(stream, "Length: %d\n", cities->size());
        cities->map(&toNames, &mCityNamesPool).printStrs(stream);
        fputc('\n', stream);

        fprintf(stream, "Max allocation nodes: %d\n", mNodePool.maxAllocated);
        fprintf(stream, "Max allocation lls: %d\n", mLLPool.maxAllocated);
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

    // Used by printRoute()
    static inline ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> mCityNamesPool;

    // Used by citiesStack
    static inline ObjectPool<Node<unsigned>, 100> mNodePool;
    // Used by citiesStack
    static inline ObjectPool<Node<LinkedList<unsigned>>, 5> mLLPool;
    // Used by visitedCities
    static inline LinkedList<LinkedList<unsigned>> mCitiesStack;
public:
    /* LinkedList needs to be below ObjectPool because the objects in
     * the class are destructed from below to top!!!
     */
    LinkedList<unsigned>* cities;
};
