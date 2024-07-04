#pragma once

#include <cstdio>

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

		// TODO: get chars

		fclose(fp);
		return true;
	}

	constexpr bool parseInput() noexcept
	{
		return true;
	}

private:

};