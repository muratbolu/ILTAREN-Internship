#pragma once

#include <cstdio>

#include "StaticVector.tpp"

// 2^16 char size buffer
#define BUF_SIZE (1024 * 64)

// 2^10 line size buffer
#define LINE_SIZE (1024)

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
		// number calculates the number for each semicolon separated
		// value
		int number{ 0 };

		enum parseStates
		{
			SKIP_INIT,
			CALC_NUM,
		} state{ SKIP_INIT };

		for (char c : buffer)
		{
			if (c == '\0') break;

			switch (state)
			{
			case SKIP_INIT:
			{
				if (c == ';') ++sc;
				if (sc == 2) state = CALC_NUM;
				break;
			}
			case CALC_NUM:
			{
				if (c == ';')
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
					state = SKIP_INIT;
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

	constexpr void printAdjacencyMatrix() noexcept
	{
		for (int i{ 0 }; auto l : adjacencyMatrix)
		{
			if (i < 9)
			{
				fprintf(stdout, "%d", 0);
			}
			// TODO: print the name of the city in between ;;
			fprintf(stdout, "%d;;", ++i);
			for (int j{ 0 }; auto n : l)
			{
				if (++j == 81)
				{
					fprintf(stdout, "%d", n);
				}
				else {
					fprintf(stdout, "%d;", n);
				}
			}
			fprintf(stdout, "\n");
		}
	}

private:
	StaticVector<char, BUF_SIZE> buffer;
	StaticVector<StaticVector<unsigned, 81>, 81> adjacencyMatrix;
};