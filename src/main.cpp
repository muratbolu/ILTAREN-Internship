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

    unsigned currMax { 0 };
    // TODO
    for (unsigned i { 0 }; i < 200; i += 10)
    {
        for (unsigned j { 0 }; j <= i; j += 10)
        {
            t.x = i;
            t.y = j;
            t.filterByRange(t.filteredAdjacencyMatrix, t.adjacencyMatrix);
            t.travel();
            if (t.validator(t.mBestState.citiesStack))
            {
                if (t.mBestState.visitedCount > currMax)
                {
                    t.printRoute(stdout);
                    currMax = t.mBestState.visitedCount;
                    printf("x: %d, y: %d\n\n", t.x, t.y);
                }
            }
            else
            {
                puts("Invalid route");
            }
        }
    }
    return 0;
}
