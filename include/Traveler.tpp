#pragma once

#include "StaticStack.tpp"
#include "StaticVector.tpp"

#include <array>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <utility>
#include <vector>

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
        mCities.fill(UINT_MAX);

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
        mCities[0] = mStartCity;
        visitableCities(mStartCity);   // writes to cities
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

    /* Tries to find the longest path by picking the neighbor
     * within the largest strongly connected component.
     */
    void visitableCities(unsigned n) noexcept
    {
        unsigned currCity { n };
        StaticVector<bool, 81> visitedArr;
        visitedArr.fill(false);
        visitedArr[currCity] = true;

        unsigned currIndex { 0 };
        bool notStuck { true };
        while (notStuck)
        {
            notStuck = false;
            unsigned nextCity { static_cast<unsigned>(-1) };

            int maxVisitable { -1 };

            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (!traversable(currCity, i) || visitedArr[i] || alreadyVisited({ visitedArr, i }))
                {
                    continue;
                }

                /*
                auto sccResult { tarjanSCC(visitedArr, i) };
                printSCC(sccResult);

                int visitable { -1 };
                for (const auto& scc : sccResult)
                {
                    if (scc[i])
                    {
                        int clusterSize { static_cast<int>(scc.count(true)) };
                        if (clusterSize > visitable)
                        {
                            visitable = clusterSize;
                        }
                    }
                }
                */

                // if (visitable > maxVisitable)
                // {
                notStuck = true;
                nextCity = i;
                //    maxVisitable = visitable;
                // }
            }
            if (notStuck)
            {
                currCity = nextCity;
                mCities[++currIndex] = currCity;
                visitedArr[currCity] = true;
            }
            else
            {
                ;
            }

            // TODO: early termination heuristic
            if (countValid(mCities) > 35)
            {
                break;
            }
        }
    }

    [[nodiscard]] bool alreadyVisited(const std::pair<StaticVector<bool, 81>, unsigned>& p) noexcept
    {
        if (mVisitedConfigs.contains(p))
        {
            return true;
        }
        else
        {
            mVisitedConfigs.pushBack(p);
            return false;
        }
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

    [[nodiscard]] constexpr bool validator(const StaticVector<unsigned, 81>& cs) const noexcept
    {
        for (unsigned i { 0 }; cs[i + 1] != UINT_MAX; ++i)
        {
            if (filteredAdjacencyMatrix[cs[i]][cs[i + 1]] == UINT_MAX)
            {
                return false;
            }
        }
        for (unsigned i { 0 }; cs[i] != UINT_MAX; ++i)
        {
            for (unsigned j { i + 1 }; cs[j] != UINT_MAX; ++j)
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
        for (unsigned i { 0 }; mCities[i] != UINT_MAX; ++i)
        {
            if (i > 0)
            {
                fputs(", ", stream);
            }
            fprintf(stream, "%s", static_cast<char*>(toNames(mCities[i]).data()));
        }
        fputs("]\n", stream);
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

    static inline StaticStack<std::pair<StaticVector<bool, 81>, unsigned>, 1000> mVisitedConfigs;
public:
    // Has UINT_MAX for empty members at the end
    static inline StaticVector<unsigned, 81> mCities;
};
