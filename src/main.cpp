#include <cstdio>

#include "Traveler.tpp"

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		int rc = fputs(
			"Provide exactly four arguments: input file, start city code, X, and Y.\n", stderr);
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
	t.getStartCity(argv[2]);
	t.getRange(argv[3], argv[4]);
	// t.filterByRange();
	// t.printAdjacencyMatrix();
	t.travel();
	t.printRoute(stdout);

	return 0;
}
