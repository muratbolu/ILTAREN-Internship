#pragma once

#include "StaticStack.tpp"
#include "StaticVector.tpp"

#include <array>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <tuple>
#include <vector>

// Separator is semicolon for our file
#define SEP ';'

// 2^16 char size buffer
#define BUF_SIZE (1024 * 64)

// max 32 characters for city names
#define MAX_NAME_SIZE (32)

class Traveler
{
    // Put the type definitions to top
    using Frame = std::tuple<StaticStack<unsigned, 81>, StaticVector<bool, 81>, unsigned>;
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

    void printSCC(const std::vector<StaticVector<bool, 81>>& val) const noexcept
    {
        for (const auto& p : val)
        {
            bool notPrinted { true };
            printf("[");
            for (unsigned i { 0 }; i < p.size(); ++i)
            {
                if (p[i])
                {
                    if (notPrinted)
                    {
                        printf("%s", static_cast<char*>(toNames(i).data()));
                        notPrinted = false;
                    }
                    else
                    {
                        printf(", %s", static_cast<char*>(toNames(i).data()));
                    }
                }
            }
            printf("]\n");
        }
        printf("\n");
    }

    // TODO: Eliminate stack allocations

    /* Tries to find the longest path by brute force, caching previously
     * visited configurations in mVisitedStack.
     */
    void visitableCities(unsigned n) noexcept
    {
        mCities.clear();
        mCities.pushBack(n);

        StaticVector<bool, 81> visitedArr;
        visitedArr.fill(false);
        visitedArr[n] = true;

        mCurrStack.pushBack({ mCities, visitedArr, n });
        while (!mCurrStack.empty())
        {
            const auto& [cities, visited, currCity] { mCurrStack.popBack() };
            if (visited.count(true) > 62)   // TODO: early termination heuristic
            {
                mCities = cities;
                return;
            }
            if (stackContains(mVisitedStack, { cities, visited, currCity }))
            {
                continue;
            }
            mVisitedStack.pushBack({ cities, visited, currCity });
            StaticStack<unsigned, 81> validCities;
            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (traversable(currCity, i) && !visited[i])
                {
                    validCities.pushBack(i);
                }
            }
            if (!validCities.empty())
            {
                for (unsigned i { 0 }; i < validCities.size(); ++i)
                {
                    StaticStack<unsigned, 81> newCities { cities };
                    newCities.pushBack(validCities[i]);
                    StaticVector<bool, 81> newVisited { visited };
                    newVisited[validCities[i]] = true;
                    mCurrStack.pushBack({ newCities, newVisited, validCities[i] });
                }
            }
        }
    }

    template<unsigned U>
    [[nodiscard]] constexpr bool stackContains(const StaticStack<Frame, U>& stack, const Frame& f) const noexcept
    {
        for (unsigned i { 0 }; i < stack.size(); ++i)
        {
            if (isSubsetOf(get<1>(stack[i]), get<1>(f)) && get<2>(stack[i]) == get<2>(f))
            {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] constexpr bool isSubsetOf(const StaticVector<bool, 81>& fst, const StaticVector<bool, 81>& snd) const noexcept
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

    /* Makes a stack allocation. Return the reachable states, not including
     * those in the visitedArr, starting from n.
     */
    [[nodiscard]] constexpr StaticVector<bool, 81> dfs(const StaticVector<bool, 81>& visitedArr, const unsigned& n) const noexcept
    {
        StaticVector<bool, 81> reachables;
        reachables.fill(false);

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

    /* Returns the vector of strongly connected components.
     * The algorithm starts from n, ignoring all nodes in visitedArr.
     */
    [[nodiscard]] std::vector<StaticVector<bool, 81>> tarjanSCC(const StaticVector<bool, 81>& visitedArr, const unsigned& n) const noexcept
    {
        std::vector<StaticVector<bool, 81>> sccs;

        std::array<int, 81> discoveryTime;
        discoveryTime.fill(-1);

        std::array<int, 81> low;
        low.fill(-1);

        std::array<bool, 81> onStack;
        onStack.fill(false);

        std::vector<unsigned> stack;

        int time = 0;

        std::function<void(unsigned)> tarjanDFS = [&](unsigned v)
        {
            discoveryTime[v] = low[v] = time++;
            stack.push_back(v);
            onStack[v] = true;

            for (unsigned u = 0; u < 81; ++u)
            {
                if (!traversable(v, u) || visitedArr[u])
                {
                    continue;
                }

                if (discoveryTime[u] == -1)
                {
                    // u is not visited, recurse on it
                    tarjanDFS(u);
                    low[v] = std::min(low[v], low[u]);
                }
                else if (onStack[u])
                {
                    // u is in the current stack
                    low[v] = std::min(low[v], discoveryTime[u]);
                }
            }

            // If v is a root node, pop the stack and generate an SCC
            if (low[v] == discoveryTime[v])
            {
                StaticVector<bool, 81> scc = {};
                unsigned w = 0;
                do
                {
                    w = stack.back();
                    stack.pop_back();
                    onStack[w] = false;
                    scc[w] = true;
                } while (w != v);
                sccs.push_back(scc);
            }
        };
        // Initialize the DFS from the starting node n
        tarjanDFS(n);
        // Check if any nodes were missed and run DFS for them as well
        for (unsigned i = 0; i < 81; ++i)
        {
            if (!visitedArr[i] && discoveryTime[i] == -1)
            {
                tarjanDFS(i);
            }
        }
        return sccs;
    }

    /*
    [[nodiscard]] constexpr unsigned edges(const StaticVector<bool, 81>& visitedArr, const unsigned& n) const noexcept
    {
        unsigned edgeCount { 0 };
        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (traversable(n, i) && !visitedArr[i])
            {
                ++edgeCount;
            }
        }
        return edgeCount;
    }
    */

    [[nodiscard]] constexpr static unsigned countValid(const StaticVector<unsigned, 81>& v) noexcept
    {
        return v.size() - v.count(UINT_MAX);
    }

    /*
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
    */

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

    void printRoute(FILE* stream) const noexcept
    {
        fprintf(stream, "Length: %d\n", countValid(mCities));
        fputs("[", stream);
        for (unsigned i { 0 }; i < mCities.size(); ++i)
        {
            if (i > 0)
            {
                fputs(", ", stream);
            }
            fprintf(stream, "%s", static_cast<char*>(toNames(mCities[i]).data()));
        }
        fputs("]\n", stream);
        fprintf(stream, "Max mCurrStack usage: %d\n", mCurrStack.mMaxIndex);
        fprintf(stream, "Max mVisitedStack usage:%d\n", mVisitedStack.mMaxIndex);
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

    static inline StaticStack<Frame, 5000> mCurrStack;
    static inline StaticStack<Frame, 10000> mVisitedStack;
public:
    // Has UINT_MAX for empty members at the end
    static inline StaticStack<unsigned, 81> mCities;
};
