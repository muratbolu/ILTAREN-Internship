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
	Traveler t{ argv[1], argv[2], argv[3], argv[4] };
	t.filterByRange(t.filteredAdjacencyMatrix, t.adjacencyMatrix);
	t.printMatrix(stdout, t.adjacencyMatrix);
	t.printMatrix(stdout, t.filteredAdjacencyMatrix);
	t.travel();
	if (t.validator(t.cities))
	{
		puts("Valid route");
		t.printRoute(stdout);
	}
	else
	{
		puts("Invalid route");
	}
	return 0;
}
