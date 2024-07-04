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

	constexpr void filterByRange() noexcept
	{
		for (auto& l : adjacencyMatrix)
		{
			for (auto& n : l)
			{
				if ((n < (X - Y)) || (n > (X + Y)))
				{
					n = UINT_MAX;
				}
			}
		}
	}

	// Zero-indexing for cities, 0 => ADANA, etc.
	void travel() noexcept
	{
		ObjectPool<Node<unsigned>, 500> stackPool;
		LinkedList<unsigned> citiesStack{ stackPool };

		citiesStack.push(startCity);

		while (cities.size() < 81 && citiesStack.size() > 0)
		{
			unsigned nextCity{ *citiesStack.pop() };
			if (cities.contains(nextCity))
			{
				continue;
			}
			cities.push(nextCity);
			for (int i{ 0 }; unsigned c : adjacencyMatrix[*cities.back()])
			{
				if (c < UINT_MAX && !cities.contains(c))
				{
					// Maybe buggy: pushes cities multiple times
					citiesStack.push(i);
				}
				++i;
			}
		}

		fprintf(stdout, "%d\n", cities.size());

		// TODO: These are for debugging. Remove
		/*
		ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> pool;
		citiesStack.map(&toNames, pool).printStrs(stdout);
		fputc('\n', stdout);
		*/

	}

	void printRoute(FILE* stream) const noexcept
	{
		// allocate a pool for city names
		ObjectPool<Node<StaticVector<char, MAX_NAME_SIZE>>, 81> pool;

		// map and print city names
		cities.map(&toNames, pool).printStrs(stream);
		fputc('\n', stream);
	}

	constexpr static StaticVector<char, MAX_NAME_SIZE> toNames(unsigned n) noexcept
	{
		StaticVector<char, MAX_NAME_SIZE> result;
		StaticVector<char, MAX_NAME_SIZE> cityName = cityNames[n];
		int i{ 0 };
		for (; cityName[i] != '\0'; ++i)
		{
			result[i] = cityName[i];
		}
		result[i] = '\0';
		return result;
	}

private:
	StaticVector<char, BUF_SIZE> buffer;
	static inline StaticVector<StaticVector<char, MAX_NAME_SIZE>, 81> cityNames;
	StaticVector<StaticVector<unsigned, 81>, 81> adjacencyMatrix;

	// startCity = 5, Ankara by default
	unsigned startCity{ 5 }, X{ 200 }, Y{ 50 };

	ObjectPool<Node<unsigned>, 81> pool;
	LinkedList<unsigned> cities{ pool };
};
