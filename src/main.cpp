#include <cstdio>

#include "Traveler.tpp"

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		int rc = fputs(
			"Provide exactly three arguments: input file, X, and Y.\n", stderr);
		if (rc == EOF)
		{
			perror("Invalid argument number");
		}
		return 1; // EXIT_FAILURE
	}

	Traveler t;
	if (!t.getInput(argv[1]))
	{
		return 1; // EXIT_FAILURE
	}
	t.parseInput();
	t.getRange(argv[2], argv[3]);
	t.filterByRange();
	t.printAdjacencyMatrix();

	return 0;
}
