// TRAVELER or BUS
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
#include "Bus.tpp"

int main(int argc, char* argv[])
{
    Bus b { argc, argv };
    return 0;
}
#endif
