#pragma once

#include "Bus.tpp"
#include "Chr.tpp"
#include "LinkedList.tpp"
#include "NodeIter.tpp"
#include "StaticStack.tpp"
#include "StaticVector.tpp"

#include <complex>
#include <cstdio>
#include <utility>

namespace io
{
void print(FILE* stream, int i) noexcept
{
    fprintf(stream, "%i", i);
}

void print(FILE* stream, unsigned i) noexcept
{
    fprintf(stream, "%u", i);
}

void print(FILE* stream, char i) noexcept
{
    fprintf(stream, "%c", i);
}

void print(FILE* stream, float i) noexcept
{
    fprintf(stream, "%.2f", i);
}

void print(FILE* stream, const chr::Time& t) noexcept
{
    fprintf(stream, "%02hhu:%02hhu", t.getHour(), t.getMinute());
}

void print(FILE* stream, const chr::Duration& d) noexcept
{
    fprintf(stream, "%u", d.getDuration());
}

template<typename T1, typename T2>
void print(FILE* stream, const std::pair<T1, T2>& p) noexcept
{
    fputc('(', stream);
    io::print(stream, p.first);
    fputs(", ", stream);
    io::print(stream, p.second);
    fputc(')', stream);
}

void print(FILE* stream, const std::complex<float>& c) noexcept
{
    fprintf(stream, "(%.2f,%.2f)", c.real(), c.imag());
}

void print(FILE* stream, const Bus& b) noexcept
{
    if (b.isAlternating())
    {
        fprintf(stream, "(%d, %d)", b.getFirst(), b.getSecond());
    }
    else
    {
        fprintf(stream, "%d", b.getFirst());
    }
}

template<typename T, unsigned N>
void print(FILE* stream, const StaticVector<T, N>& sv) noexcept
{
    fputs("[", stream);
    for (unsigned i { 0 }; i < N; ++i)
    {
        io::print(stream, sv[i]);
        if (i + 1 < N)
        {
            fputs(", ", stream);
        }
    }
    fputs("]", stream);
}

template<typename T, unsigned N>
void print(FILE* stream, const StaticStack<T, N>& sv) noexcept
{
    fputs("[", stream);
    for (unsigned i { 0 }; i < sv.size(); ++i)
    {
        io::print(stream, sv[i]);
        if (i + 1 < sv.size())
        {
            fputs(", ", stream);
        }
    }
    fputs("]", stream);
}

template<typename T>
void print(FILE* stream, const LinkedList<T>& ll) noexcept
{
    fputs("[", stream);
    for (NodeIter<T> curr { ll.begin() }; curr != ll.end();)
    {
        io::print(stream, curr.data());
        if (curr.next() != ll.end())
        {
            fputs(", ", stream);
        }
    }
    fputs("]", stream);
}
}   // namespace io
