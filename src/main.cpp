// TRAVELER or BUS
#include <cstdlib>
#define BUS

#ifdef TRAVELER
#include "Traveler.tpp"

int main(int argc, char* argv[])
{
    Traveler t { argc, argv };
    return 0;
}
#endif

#ifdef BUS
#include "BusSchedule.tpp"
#include "Maybe.tpp"

#include <cstdio>

int main(int argc, char* argv[])
{
    Maybe<BusSchedule> b { BusSchedule::create(argc, argv) };
    if (!b.exists())
    {
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "w");   // NOLINT
    if (fp == nullptr)
    {
        perror("Could not open file");
        return EXIT_FAILURE;
    }

    b.data().printArrivals(fp);
    return 0;
}
#endif
