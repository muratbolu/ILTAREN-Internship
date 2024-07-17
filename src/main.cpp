#include "StaticVector.tpp"
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
    static Traveler t { argv[1], argv[2], argv[3], argv[4] };

    unsigned currMax { 0 };
    // i = x + y
    for (unsigned i { 0 }; i < 300; ++i)
    {
        // j = y
        for (unsigned j { 0 }; j <= i / 2; ++j)
        {
            t.x = i - j;
            t.y = j;
            t.filterByRange(t.mFilteredAdjacencyMatrix, t.mAdjacencyMatrix);
            if (t.matIsSubsetOf(t.mFilteredAdjacencyMatrix, t.mBestMat))
            {
                continue;
            }
            t.travel();
            if (t.validator(t.mBestState.citiesStack))
            {
                if (t.mBestState.visitedCount > currMax)
                {
                    t.printRoute(stdout);
                    currMax = t.mBestState.visitedCount;
                    t.mBestMat = t.mFilteredAdjacencyMatrix;
                    printf("x: %d, y: %d\n\n", t.x, t.y);
                }
                if (currMax == 81)
                {
                    return 0;
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
