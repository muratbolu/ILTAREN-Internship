#include <cstdio>

#include "Traveler.tpp"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		int rc = fputs(
			"Provide exactly one argument, which should be the input file.\n", stderr);
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
	t.filterByRange(150, 250);
	t.printAdjacencyMatrix();

	return 0;
}
