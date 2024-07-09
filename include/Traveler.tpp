#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticVector.tpp"

#include <array>
#include <cassert>
#include <climits>
// #include <compare>   // Needed because of a bug in MSVC
#include <cstdio>
#include <cstdlib>

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
        // distances.pool() = &distancesPool;

        getInput(arg1);
        parseInput();
    }

    // returns false if something goes wrong but we don't use it
    bool getInput(char arg[]) noexcept
    {
        FILE* fp = fopen(arg, "r");
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

    void preprocessMatrix() const noexcept
    {
        for (unsigned i { 0 }; i < 81; ++i)
        {
            unsigned edgeCount { 0 };
            for (unsigned j { 0 }; j < 81; ++j)
            {
                if (filteredAdjacencyMatrix[i][j] < UINT_MAX)
                {
                    edgeCount++;
                }
            }
            edgeCounts[i] = edgeCount;
        }
    }

    // Zero-indexing for cities, 0 => ADANA, etc.
    void travel() noexcept
    {
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* startCityList = citiesStack.back();
        startCityList->push_back(startCity);

        cities = visitableCities(startCityList);
        // cities = visitableCities(cities);
        // cities = kahnsAlgorithm();
        // shortestPath(cities);
        // printf("Length: %d\n", distances.size());
        // distances.printNums(stdout);
        // putc('\n', stdout);
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

    LinkedList<unsigned>* visitableCities(const LinkedList<unsigned>* visited) const noexcept
    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* bestSoFar = citiesStack.back();

        // the actual computation goes here
        *bestSoFar = *visited;

        printf("Length: %d\n", bestSoFar->size());
        bestSoFar->map(&Traveler::toNames, &Traveler::cityNamesPool).printStrs(stdout);

        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (traversable(*visited->back(), i) && !visited->contains(i))
            {
                citiesStack.push_back(LinkedList<unsigned> { &nodePool });
                *citiesStack.back() = *visited;
                citiesStack.back()->push_back(i);

                LinkedList<unsigned>* candidate = visitableCities(citiesStack.back());

                putc('\n', stdout);

                if (candidate->size() > bestSoFar->size())
                {
                    *bestSoFar = *candidate;
                }
                citiesStack.pop_back();   // pop candidate
                citiesStack.pop_back();   // pop augmented visited list
                if (bestSoFar->size() > 40)
                {
                    break;
                }
            }
        }

        // return the first push_back
        return bestSoFar;
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

    LinkedList<unsigned>* visitableCitiesImprove(const LinkedList<unsigned>* visited, const LinkedList<unsigned>* banned) const noexcept
    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* bestSoFar = citiesStack.back();

        // the actual computation goes here
        *bestSoFar = *visited;
        bool sameSolution { true };
        for (unsigned i { 0 }; i < 81; ++i)
        {
            if (traversable(*visited->back(), i) && !visited->contains(i) && !banned->contains(i))
            {
                citiesStack.push_back(LinkedList<unsigned> { &nodePool });
                *citiesStack.back() = *visited;
                citiesStack.back()->push_back(i);

                LinkedList<unsigned>* candidate = visitableCitiesImprove(citiesStack.back(), banned);
                if (candidate->size() > bestSoFar->size())
                {
                    sameSolution = false;
                    *bestSoFar = *candidate;
                    if (bestSoFar->size() > 65)
                    {
                        printf("Best so far: %d\n", bestSoFar->size());
                    }
                }
                citiesStack.pop_back();   // pop candidate
                citiesStack.pop_back();   // pop augmented visited list
                if (bestSoFar->size() > 65)
                {
                    break;
                }
            }
        }
        if (sameSolution && visited->size() > 40)
        {
            citiesStack.push_back(LinkedList<unsigned> { &nodePool });
            LinkedList<unsigned>* bestCopy = citiesStack.back();
            *bestCopy = *bestSoFar;

            citiesStack.push_back(LinkedList<unsigned> { &nodePool });
            LinkedList<unsigned>* bannedCopy = citiesStack.back();
            *bannedCopy = *banned;

            bannedCopy->push_back(*bestCopy->pop_back());
            /*
            LinkedList<unsigned>* candidate = visitableCitiesImprove(bestCopy, bannedCopy);
            if (candidate->size() > bestSoFar->size())
            {
                *bestSoFar = *candidate;
                if (bestSoFar->size() > 60)
                {
                    printf("Best so far: %d\n", bestSoFar->size());
                }
            }
            citiesStack.pop_back();   // pop candidate
            */

            citiesStack.pop_back();   // pop bannedCopy
            citiesStack.pop_back();   // pop bestCopy
        }

        // return the first push_back
        return bestSoFar;
    }

    LinkedList<unsigned>* visitableCitiesLongestDistanceHeuristic(const LinkedList<unsigned>* visited) const noexcept

    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* longestDistances = citiesStack.back();

        // the actual computation goes here
        *longestDistances = *visited;
        bool notStuck { true };
        while (notStuck)
        {
            notStuck = false;
            unsigned currNext { static_cast<unsigned>(-1) };
            unsigned currMax { 0 };
            for (unsigned i { 0 }; i < 81; ++i)
            {
                unsigned dist { filteredAdjacencyMatrix[*longestDistances->back()][i] };
                if (traversable(*longestDistances->back(), i) && dist > currMax && !longestDistances->contains(i))
                {
                    currNext = i;
                    currMax = dist;
                    notStuck = true;
                }
            }
            if (notStuck)
            {
                longestDistances->push_back(currNext);
            }
        }

        // return the first push_back
        return longestDistances;
    }

    LinkedList<unsigned>* visitableCitiesShortestDistanceHeuristic(const LinkedList<unsigned>* visited) const noexcept
    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* shortestDistances = citiesStack.back();

        // the actual computation goes here
        *shortestDistances = *visited;
        bool notStuck { true };
        while (notStuck)
        {
            notStuck = false;
            unsigned currNext { static_cast<unsigned>(-1) };
            unsigned currMin { UINT_MAX };
            for (unsigned i { 0 }; i < 81; ++i)
            {
                unsigned dist { filteredAdjacencyMatrix[*shortestDistances->back()][i] };
                if (dist < currMin && !shortestDistances->contains(i))
                {
                    currNext = i;
                    currMin = dist;
                    notStuck = true;
                }
            }
            if (notStuck)
            {
                shortestDistances->push_back(currNext);
            }
        }

        // return the first push_back
        return shortestDistances;
    }

    LinkedList<unsigned>* visitableCitiesMostEdgesHeuristic(const LinkedList<unsigned>* visited) const noexcept
    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* mostEdges = citiesStack.back();

        // the actual computation goes here
        *mostEdges = *visited;
        bool notStuck { true };
        while (notStuck)
        {
            notStuck = false;
            unsigned currNext { static_cast<unsigned>(-1) };
            unsigned currMax { 0 };
            for (unsigned i { 0 }; i < 81; ++i)
            {
                unsigned dist { filteredAdjacencyMatrix[*mostEdges->back()][i] };
                if (traversable(*mostEdges->back(), i) && edgeCounts[i] > currMax && !mostEdges->contains(i))
                {
                    currNext = i;
                    currMax = edgeCounts[i];
                    notStuck = true;
                }
            }
            if (notStuck)
            {
                mostEdges->push_back(currNext);
                for (unsigned i { 0 }; i < 81; ++i)
                {
                    if (traversable(currNext, i))
                    {
                        edgeCounts[i]--;
                    }
                }
            }
        }

        // return the first push_back
        return mostEdges;
    }

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

    /* Calculates the number of visitable nodes from the last node in ll,
     * not visiting any of the nodes in ll.
     */
    unsigned depthFirstSearch(const LinkedList<unsigned>* ll) const noexcept
    {
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* cityStack = citiesStack.back();

        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* visited = citiesStack.back();
        *visited = *ll;

        cityStack->push_back(*visited->pop_back());
        while (cityStack->size() > 0)
        {
            visited->push_back(*cityStack->pop_back());
            for (unsigned i { 0 }; i < 81; ++i)
            {
                if (traversable(*visited->back(), i) && !visited->contains(i))
                {
                    cityStack->push_back(i);
                }
            }
        }

        unsigned result { visited->size() - ll->size() };

        citiesStack.pop_back();
        citiesStack.pop_back();

        return result;
    }

    LinkedList<unsigned>* kahnsAlgorithm() const noexcept
    {
        // the first push_back is the return value.
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* result = citiesStack.back();

        result->push_back(startCity);
        unsigned i { 0 };
        do
        {
            for (unsigned j { 0 }; j < 81; ++j)
            {
                unsigned dist { filteredAdjacencyMatrix[*result->at(i)][j] };
                if (dist < UINT_MAX && !result->contains(j))
                {
                    result->push_back(j);
                }
            }
            ++i;
        } while (i < result->size());

        // return the first push_back
        return result;
    }

    /*
    // writes the result to distances
    void shortestPath(const LinkedList<unsigned>* ts) noexcept
    {
        for(unsigned i{0}; i < 81; ++i)
        {
            distances.push_back(INT_MAX);
        }
        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* visited = citiesStack.back();

        citiesStack.push_back(LinkedList<unsigned> { &nodePool });
        LinkedList<unsigned>* topSort = citiesStack.back();
        *topSort = *ts;

        *distances.at(*topSort->front()) = 0;

        while(topSort->size() > 0)
        {
            visited->push_back(*topSort->pop_front());
            assert(*distances.at(*visited->back()) <= 0);
            for(unsigned i{0}; i < 81; ++i)
            {
                unsigned dist { filteredAdjacencyMatrix[*visited->back()][i] };
                if (dist < UINT_MAX
                // && !visited->contains(i)
                && *distances.at(*visited->back()) <= *distances.at(i))
                {
                    *distances.at(i) = *distances.at(*visited->back()) - 1;
                }
            }
        }

        citiesStack.pop_back();
        citiesStack.pop_back();
    }
    */

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
        cities->map(&Traveler::toNames, &Traveler::cityNamesPool).printStrs(stream);
        fputc('\n', stream);

        fprintf(stream, "Max allocation nodes: %d\n", nodePool.maxAllocated);
        fprintf(stream, "Max allocation lls: %d\n", llPool.maxAllocated);
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

    // Filled in by constructor
    unsigned startCity, X, Y;

    // Used by parseInput()
    StaticVector<char, BUF_SIZE> buffer;

    // Filled in by parseInput()
    static inline StaticVector<StaticVector<char, MAX_NAME_SIZE>, 81> cityNames;

    // Used by printRoute()
    static inline ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> cityNamesPool;

    static inline ObjectPool<Node<unsigned>, 8000> nodePool;
    static inline ObjectPool<Node<LinkedList<unsigned>>, 200> llPool;
    static inline LinkedList<LinkedList<unsigned>> citiesStack;

    static inline StaticVector<unsigned, 81> edgeCounts;

    // static inline ObjectPool<Node<int>, 81> distancesPool;
    // static inline LinkedList<int> distances;
public:
    /* LinkedList needs to be below ObjectPool because the objects in
     * the class are destructed from below to top!!!
     */
    LinkedList<unsigned>* cities { nullptr };
};
