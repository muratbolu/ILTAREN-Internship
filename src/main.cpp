#include "Traveler.tpp"

#include <cassert>
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
    // t.printMatrix(stdout, t.mAdjacencyMatrix);
    Traveler::printMatrixInfo(Traveler::mAdjacencyMatrix);
    return 0;

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
            assert(X > 0);
            assert(Y > 0);
            Traveler::mX = static_cast<unsigned>(X);
            Traveler::mY = static_cast<unsigned>(Y);
            // printf("%d:  t.x: %d, t.y: %d\n", i, t.x, t.y);
            Traveler::filterByRange(Traveler::mFilteredAdjacencyMatrix, Traveler::mAdjacencyMatrix);
            if (Traveler::matIsSubsetOf(Traveler::mFilteredAdjacencyMatrix, Traveler::mBestMat))
            {
                continue;
            }
            Traveler::travel();
            if (Traveler::validator(Traveler::mBestState.citiesStack))
            {
                static unsigned currMax { 0 };
                if (Traveler::mBestState.visitedCount > currMax)
                {
                    Traveler::printRoute(stdout);
                    currMax = Traveler::mBestState.visitedCount;
                    Traveler::mBestMat = Traveler::mFilteredAdjacencyMatrix;
                    printf("x: %d, y: %d\n\n", Traveler::mX, Traveler::mY);
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
