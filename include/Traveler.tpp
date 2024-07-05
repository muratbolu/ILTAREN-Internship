#pragma once

#include <climits>
#include <compare> // Needed because of a bug in MSVC
#include <cstdio>
#include <cstdlib>

#include "LinkedList.tpp"
#include "ObjectPool.tpp"
#include "StaticVector.tpp"

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
	constexpr Traveler() noexcept = default;

	bool getInput(char arg[]) noexcept
	{
		FILE* fp = fopen(arg, "r");
		if (fp == nullptr)
		{
			perror("Could not open file");
			return false;
		}

		for (int c; char& i : buffer)
		{
			if ((c = fgetc(fp)) == EOF)
			{
				break;
			}
			i = c;
		}

		if (ferror(fp))
		{
			perror("I/O error while reading file");
			fclose(fp);
			return false;
		}
		else if (feof(fp))
		{
			/*
			puts("Successfully read file: ");
			puts(arg);
			*/
			fclose(fp);
			return true;
		}
		else
		{
			puts("No error and didn't reach EOF");
			fclose(fp);
			return false;
		}

	}

	// Assumes that the input is well-formed.
	constexpr void parseInput() noexcept
	{
		// sc keeps tracks of how many semicolons are encountered
		int sc{ 0 };
		// line keeps track of the line number
		int line{ 0 };
		// i keeps track of the character index in city names
		int i{ 0 };
		// number calculates the number for each semicolon separated
		// value
		int number{ 0 };

		enum parseStates
		{
			SKIP_CODE,
			GET_NAME,
			CALC_NUM,
		} state{ SKIP_CODE };

		for (char c : buffer)
		{
			if (c == '\0') break;

			switch (state)
			{
			case SKIP_CODE:
			{
				if (c == SEP)
				{
					++sc;
					i = 0;
					state = GET_NAME;
				}
				break;
			}
			case GET_NAME:
			{
				if (c == SEP)
				{
					cityNames[line][i++] = '\0';
					++sc;
					state = CALC_NUM;
				}
				else
				{
					cityNames[line][i++] = c;
				}
				break;
			}
			case CALC_NUM:
			{
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
					number += (c - '0');
				}
				break;
			}
			}
		}
	}

	void getStartCity(const char* arg) noexcept
	{
		startCity = atoi(arg) - 1;
	}

	void getRange(const char* arg1, const char* arg2) noexcept
	{
		X = atoi(arg1);
		Y = atoi(arg2);
	}

	constexpr void printAdjacencyMatrix() noexcept
	{
		for (int i{ 0 }; auto & l : adjacencyMatrix)
		{
			fprintf(stdout, "%02d", ++i);
			fputc(SEP, stdout);
			fprintf(stdout, "%s", cityNames[i - 1].data());
			fputc(SEP, stdout);
			for (int j{ 0 }; auto & n : l)
			{
				fprintf(stdout, "%d", n);
				if (++j != 81)
				{
					fputc(SEP, stdout);
				}
			}
			fprintf(stdout, "\n");
		}
	}

	constexpr void printFilteredAdjacencyMatrix() noexcept
	{
		for (int i{ 0 }; auto & l : filteredAdjacencyMatrix)
		{
			fprintf(stdout, "%02d", ++i);
			fputc(SEP, stdout);
			fprintf(stdout, "%s", cityNames[i - 1].data());
			fputc(SEP, stdout);
			for (int j{ 0 }; auto & n : l)
			{
				fprintf(stdout, "%d", n);
				if (++j != 81)
				{
					fputc(SEP, stdout);
				}
			}
			fprintf(stdout, "\n");
		}
	}

	constexpr void filterByRange() noexcept
	{
		for (unsigned i{ 0 }; i < 81; ++i)
		{
			for (unsigned j{ 0 }; j < 81; ++j)
			{
				if ((adjacencyMatrix[i][j] < (X - Y)) || (adjacencyMatrix[i][j] > (X + Y)))
				{
					filteredAdjacencyMatrix[i][j] = UINT_MAX;
				}
				else {
					filteredAdjacencyMatrix[i][j] = adjacencyMatrix[i][j];
				}
			}
		}
	}

	// Zero-indexing for cities, 0 => ADANA, etc.
	void travel() noexcept
	{
		ObjectPool<Node<unsigned>, 10000> stackPool;
		LinkedList<unsigned> visited{ stackPool };
		visited.push(startCity);
		cities = visitableCities(visited);
	}

	// TODO: eliminate stack allocations.
	// TODO: eliminate recursive methods.

	/* Returns the longest path of cities visitable, not visiting the
	 * previously-visited cities. Exists early if a sufficiently long
	 * route is found.
	 */
	LinkedList<unsigned> visitableCities(const LinkedList<unsigned>& visited) noexcept
	{
		// keep a local copy
		LinkedList<unsigned> myVisited{ visited };
		// remembers the best route seen so far
		LinkedList<unsigned> best{ myVisited };
		// Iterates through all neighbors
		for (unsigned i{ 0 }; i < 81; ++i)
		{
			unsigned dist{ filteredAdjacencyMatrix[*myVisited.back()][i] };
			// if distance is valid and the city is not visited before
			if (dist < UINT_MAX && !myVisited.contains(i))
			{
				if (!myVisited.push(i))
				{
					return best;
				}
				// pass-by-value, so visited is intact
				LinkedList<unsigned> temp{ visitableCities(myVisited) };
				if (temp.size() > 65)
				{
					return temp;
				}
				if (temp.size() > best.size())
				{
					best = temp;
				}
				myVisited.pop();
			}
		}
		return best;
	}

	constexpr bool validator(const LinkedList<unsigned>& cs) const noexcept
	{
		for (unsigned i{ 0 }; (i + 1) < cs.size(); ++i)
		{
			if (filteredAdjacencyMatrix[*cs[i]][*cs[i + 1]] == UINT_MAX)
			{
				return false;
			}
		}
		for (unsigned i{ 0 }; i < cs.size(); ++i)
		{
			for (unsigned j{ i + 1 }; j < cs.size(); ++j)
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
		// allocate a pool for city names
		ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> pool;

		fprintf(stream, "Length: %d\n", cities.size());
		// map and print city names
		cities.map(&toNames, pool).printStrs(stream);
		fputc('\n', stream);
	}

	constexpr static StaticVector<char, MAX_NAME_SIZE> toNames(unsigned n) noexcept
	{
		return cityNames[n];
	}

private:
	StaticVector<char, BUF_SIZE> buffer;
	static inline StaticVector<StaticVector<char, MAX_NAME_SIZE>, 81> cityNames;
	StaticVector<StaticVector<unsigned, 81>, 81> adjacencyMatrix;
	StaticVector<StaticVector<unsigned, 81>, 81> filteredAdjacencyMatrix;
	unsigned startCity, X, Y;
	ObjectPool<Node<unsigned>, 81> pool;
public:
	/* LinkedList needs to be below ObjectPool because the objects in
	 * the class are destructed from below to top!!!
	 */
	LinkedList<unsigned> cities{ pool };
};
