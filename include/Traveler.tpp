#pragma once

#include "LinkedList.tpp"
#include "Node.tpp"
#include "ObjectPool.tpp"
#include "StaticVector.tpp"

#include <climits>
#include <compare>   // Needed because of a bug in MSVC
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
        cities.pool() = &pool;

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

    // Zero-indexing for cities, 0 => ADANA, etc.
    void travel() noexcept
    {
        cities.push_back(5);
        cities.push_back(25);
    }

    // TODO: eliminate stack allocations.
    // TODO: eliminate recursive methods.

    /* Returns the longest path of cities visitable, not visiting the
     * previously-visited cities. Exists early if a sufficiently long
     * route is found.
     */
    /*
    LinkedList<unsigned>& visitableCities(LinkedList<unsigned>& visitedPtr) noexcept
    {
        // remembers the best route seen so far
        LinkedList<unsigned>& bestPtr { *linkedListPool.allocate() };
        bestPtr = visitedPtr;

        // Iterates through all neighbors
        for (unsigned i { 0 }; i < 81; ++i)
        {
            unsigned dist { filteredAdjacencyMatrix[*visitedPtr.back()][i] };
            // if distance is valid and the city is not visited before
            if (dist < UINT_MAX && !visitedPtr.contains(i))
            {
                if (!visitedPtr.push(i))
                {
                    puts("out of memory");
                    return bestPtr;
                }
                LinkedList<unsigned>& tempPtr { *linkedListPool.allocate() };
                tempPtr = visitableCities(visitedPtr);
                if (tempPtr.size() > 65)
                {
                    // bestPtr.~LinkedList();
                    linkedListPool.deallocate(&bestPtr);
                    return tempPtr;
                }
                if (tempPtr.size() > bestPtr.size())
                {
                    bestPtr = tempPtr;
                }
                // tempPtr.~LinkedList();
                linkedListPool.deallocate(&tempPtr);
                visitedPtr.pop();
            }
        }

        return bestPtr;
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
        fprintf(stream, "Length: %d\n", cities.size());
        cities.map(&Traveler::toNames, &Traveler::cityNamesPool).printStrs(stream);
        fputc('\n', stream);
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

    // Filled in by constructor
    unsigned startCity, X, Y;

    // Used by parseInput()
    StaticVector<char, BUF_SIZE> buffer;

    // Filled in by parseInput()
    static inline StaticVector<StaticVector<char, MAX_NAME_SIZE>, 81> cityNames;

    // Used by printRoute()
    static inline ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> cityNamesPool;

    static inline ObjectPool<Node<unsigned>, 81> pool;
public:
    /* LinkedList needs to be below ObjectPool because the objects in
     * the class are destructed from below to top!!!
     */
    LinkedList<unsigned> cities;
};
