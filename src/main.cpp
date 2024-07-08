#include "Traveler.tpp"

#include <cstdio>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        int rc = fputs("Provide exactly four arguments: input file, start city "
                       "code, X, and Y.\n",
                       stderr);
        if (rc == EOF)
        {
            perror("Invalid argument number");
        }
        return 1;   // EXIT_FAILURE
    }
    Traveler t { argv[1], argv[2], argv[3], argv[4] };
    t.filterByRange(t.filteredAdjacencyMatrix, t.adjacencyMatrix);
    // Traveler::printMatrix(stdout, t.adjacencyMatrix);
    // Traveler::printMatrix(stdout, t.filteredAdjacencyMatrix);
    t.travel();
    t.printRoute(stdout);
    /*
    if (t.cities != nullptr && t.validator(*t.cities))
    {
        puts("Valid route");
    }
    else
    {
        puts("Invalid route");
    }
    */
    return 0;
}
