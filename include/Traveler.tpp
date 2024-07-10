#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticVector.tpp"

#include <climits>
// #include <compare>   // Needed because of a bug in MSVC
#include <cstdio>
#include <cstdlib>
#include <tuple>

// Separator is semicolon for our file
#define SEP ';'

// 2^16 char size buffer
#define BUF_SIZE (1024 * 64)

// 2^10 line size buffer
#define LINE_SIZE (1024)

// max 32 characters for city names
#define MAX_NAME_SIZE (32)

class Traveler
{
public:
    Traveler(char arg1[], char arg2[], char arg3[], char arg4[]) noexcept :
        startCity { static_cast<unsigned>(atoi(arg2) - 1) },
        X { static_cast<unsigned>(atoi(arg3)) },
        Y { static_cast<unsigned>(atoi(arg4)) }
    {
        citiesStack.pool() = &llPool;
        traversed.pool() = &traversedPool;

        getInput(arg1);
        parseInput();
    }

    // returns false if something goes wrong but we don't use it
    bool getInput(char arg[]) noexcept
    {
        FILE* fp;
        if (fopen_s(&fp, arg, "r") != 0)
        {
            perror("Could not open file");
            return false;
        }
        if (fp == nullptr)
        {
            perror("Could not open file");
            return false;
        }

        for (int c { EOF }; char& i : buffer)
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

        for (char c : buffer)
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
                    Traveler::cityNames[line][i++] = '\0';
                    ++sc;
                    state = CALC_NUM;
                }
                else
                {
                    Traveler::cityNames[line][i++] = c;
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
            fprintf(stream, "%s", Traveler::cityNames[i].data());
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
                if ((src[i][j] < (X - Y)) || (src[i][j] > (X + Y)))
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
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* startCityList = citiesStack.back();
        startCityList->push_back(startCity);

        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        cities = citiesStack.back();
        *cities = *startCityList;

        visitableCities(startCityList);   // writes to cities

        /* There must be two values on the stack now. The first
         * value is the visited cities and the second value is the
         * return value of visitableCities.
         */
        citiesStack.pop_front();
    }

    // TODO: eliminate recursive methods.

    /* Returns the longest path of cities visitable, not visiting the
     * previously-visited cities. Exists early if a sufficiently long
     * route is found. "Stack load" of visitableCities is always 1.
     */

    constexpr void visitableCities(LinkedList<unsigned>* visited) const noexcept
    {
        StaticVector<bool, 81> visitedArr { toCityArray(visited) };
        unsigned currCity { *visited->back() };

        for (unsigned i { 0 }; i < 81 && cities->size() < 68; ++i)
        {
            std::tuple triplet { visitedArr, i, DFS(visitedArr, i) };
            if (traversable(currCity, i) && !visitedArr[i] && RDD(triplet))
            {
                visited->push_back(i);
                if (visited->size() > cities->size())
                {
                    *cities = *visited;
                }
                visitableCities(visited);
                visited->pop_back();
                traversed.push_back(std::move(triplet));
            }
        }
    }

    /* TODO: Write visitableCitiesWithHeuristics that implement:
     * 1. Basic symmetry detection, assign a prime number for each city
     *    and instead of keeping a 'visited' list, just check if a city's
     *    prime number divides the return value. But how to prune states
     *    such that states with identical 'numbers' are pruned?
     * 2. Reachable dominance detection, assign a prime number for each city
     *    and if the visited of a state divides the best seen so far, prune
     *    that state.
     */

    /*
    LinkedList<unsigned>* visitableCitiesDFSHeuristic(const LinkedList<unsigned>* visited) const noexcept
    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* citiesDFS = citiesStack.back();
        *citiesDFS = *visited;

        unsigned currMaxDFS;
        do
        {
            unsigned currNext { static_cast<unsigned>(-1) };
            currMaxDFS = 0;
            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (traversable(*citiesDFS->back(), i) && !citiesDFS->contains(i))
                {
                    citiesDFS->push_back(i);
                    unsigned DFS { depthFirstSearch(citiesDFS) };
                    if (DFS > currMaxDFS)
                    {
                        currMaxDFS = DFS;
                        currNext = i;
                    }
                    citiesDFS->pop_back();
                }
            }
            if (currMaxDFS)
            {
                citiesDFS->push_back(currNext);
            }
        } while (currMaxDFS);

        // return the first push_back
        return citiesDFS;
    }
    */

    /* Calculates the number of visitable nodes from the last node in ll,
     * not visiting any of the nodes in ll.
     */
    /*
    unsigned depthFirstSearch(const LinkedList<unsigned>* ll) const noexcept
    {
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* stack = citiesStack.back();

        StaticVector<bool, 81> visitedArr { toCityArray(ll) };

        stack->push_back(*ll->back());

        unsigned visitable { 0 };
        while (stack->size() > 0)
        {
            unsigned curr { *stack->pop_back() };
            visitable++;
            visitedArr[curr] = true;
            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (traversable(curr, i) && !visitedArr[i])
                {
                    stack->push_back(i);
                }
            }
        }
        citiesStack.pop_back();
        return visitable;
    }
    */

    /* Makes a stack allocation. Return the reachable states, not including
     * those in the visitedArr, starting from n.
     * TODO: this method makes too many Node allocations. Can I reduce it?
     * I am only interested in visitable states from a particular state.
     */
    constexpr StaticVector<bool, 81> DFS(const StaticVector<bool, 81>& visitedArr, const unsigned& n) const noexcept
    {
        StaticVector<bool, 81> reachables;
        reachables.clear();

        StaticVector<unsigned, 81> stack;
        unsigned currIndex { 0 };
        stack[currIndex++] = n;
        while (currIndex)
        {
            unsigned curr { stack[currIndex--] };
            reachables[curr] = true;
            for (unsigned i { 0 }; i < 81; ++i)
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

    constexpr bool RDD(const std::tuple<const StaticVector<bool, 81>&, const unsigned&, const StaticVector<bool, 81>&>& t) const noexcept
    {
        auto countTrue = [](const StaticVector<bool, 81>& v) -> unsigned
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
        };

        unsigned visitedNum { countTrue(get<0>(t)) };
        unsigned visitableNum { countTrue(get<2>(t)) };

        if (visitedNum + 1 + visitableNum < cities->size())
        {
            traversed.push_back(t);
            return false;
        }

        for (const auto& path : traversed)
        {
            if (get<1>(path) == get<1>(t) && countTrue(get<0>(path)) >= visitedNum && isSubsetOf(get<2>(t), get<2>(path)))
            {
                return false;
            }
        }

        return true;
    }

    constexpr bool isSubsetOf(const StaticVector<bool, 81>& fst, const StaticVector<bool, 81>& snd) const noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (fst[i] == true && snd[i] == false)
            {
                return false;
            }
        }
        return true;
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
        cities->map(&toNames, &cityNamesPool).printStrs(stream);
        fputc('\n', stream);

        fprintf(stream, "Max allocation nodes: %d\n", nodePool.maxAllocated);
        fprintf(stream, "Max allocation lls: %d\n", llPool.maxAllocated);
        fprintf(stream, "Max allocation traversed: %d\n", traversedPool.maxAllocated);
    }

    // Filled in by parseInput()
    StaticVector<StaticVector<unsigned, 81>, 81> adjacencyMatrix;
    // Filled in by filterByRange()
    StaticVector<StaticVector<unsigned, 81>, 81> filteredAdjacencyMatrix;
private:
    // Used by printRoute. Must be called after parseInput()
    constexpr static StaticVector<char, MAX_NAME_SIZE> toNames(unsigned n) noexcept
    {
        return Traveler::cityNames[n];
    }

    constexpr bool traversable(unsigned n, unsigned m) const noexcept
    {
        return filteredAdjacencyMatrix[n][m] < UINT_MAX;
    }

    // Makes a stack allocation
    constexpr StaticVector<bool, 81> toCityArray(const LinkedList<unsigned>* ll) const noexcept
    {
        StaticVector<bool, 81> arr;
        arr.clear();
        for (const unsigned& c : *ll)
        {
            arr[c] = true;
        }
        return arr;
    }

    // Filled in by constructor
    unsigned startCity, X, Y;

    // Used by parseInput()
    static inline StaticVector<char, BUF_SIZE> buffer;

    // Filled in by parseInput()
    static inline StaticVector<StaticVector<char, MAX_NAME_SIZE>, 81> cityNames;

    // Used by printRoute()
    static inline ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> cityNamesPool;

    // Used by traversed
    static inline ObjectPool<Node<std::tuple<StaticVector<bool, 81>, unsigned, StaticVector<bool, 81>>>, 5000> traversedPool;
    // Used by visitedCities to cache previously visited combinations
    static inline LinkedList<std::tuple<StaticVector<bool, 81>, unsigned, StaticVector<bool, 81>>> traversed;

    // Used by citiesStack
    static inline ObjectPool<Node<unsigned>, 10000> nodePool;
    // Used by citiesStack
    static inline ObjectPool<Node<LinkedList<unsigned>>, 500> llPool;
    // Used by visitedCities
    static inline LinkedList<LinkedList<unsigned>> citiesStack;
public:
    /* LinkedList needs to be below ObjectPool because the objects in
     * the class are destructed from below to top!!!
     */
    LinkedList<unsigned>* cities;
};
