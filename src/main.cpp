#include "Traveler.tpp"

#include <cmath>
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

#define I (i)
#define J (j)
#define Y (J)                 // j = y
#define FUNC(x) ((x) * (x))   // f(x) = x^2
#define X (I - FUNC(Y))       // i = x + f(y)
#define LIM 10000

    for (int i { 0 }; i < LIM; ++i)
    {
        for (int j { 0 }; Y <= X; ++j)   // t.x >= t.y
        {
            t.x = X;
            t.y = Y;
            // printf("%d:  t.x: %d, t.y: %d\n", i, t.x, t.y);
            t.filterByRange(t.mFilteredAdjacencyMatrix, t.mAdjacencyMatrix);
            if (t.matIsSubsetOf(t.mFilteredAdjacencyMatrix, t.mBestMat))
            {
                continue;
            }
            t.travel();
            if (t.validator(t.mBestState.citiesStack))
            {
                static unsigned currMax { 0 };
                if (t.mBestState.visitedCount > currMax)
                {
                    t.printRoute(stdout);
                    currMax = t.mBestState.visitedCount;
                    t.mBestMat = t.mFilteredAdjacencyMatrix;
                    printf("x: %d, y: %d\n\n", t.x, t.y);
                    if (currMax == 81)
                    {
                        return 0;
                    }
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
