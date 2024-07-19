#pragma once

#include "Chr.tpp"
#include "LinkedList.tpp"
#include "NodeIter.tpp"

#include <cstdio>

namespace io
{
// Prints the numbers as unsigned (%u)
constexpr void print(FILE* stream, unsigned i) noexcept
{
    fprintf(stream, "%u", i);
}

constexpr void print(FILE* stream, const chr::Time& t) noexcept
{
    fprintf(stream, "%02hhu:%02hhu\n", t.getHour(), t.getMinute());
}

constexpr void print(FILE* stream, const chr::Duration& d) noexcept
{
    fprintf(stream, "%u\n", d.getDuration());
}

template<typename T>
constexpr void print(FILE* stream, const LinkedList<T>& ll) noexcept
{
    fputs("[", stream);
    for (NodeIter<T> curr { ll.begin() }; curr != ll.end(); curr.next())
    {
        io::print(stream, curr.data());
        fputs(", ", stream);
    }
    fputs("]\n", stream);
}

/*
// TODO: export print methods into friend methods in a separate file
// Only use it with NULL-terminated strings
constexpr void print(FILE* stream, unsigned i) noexcept
{
    fprintf(stream, "%s", static_cast<char*>(curr->data().data()));
}
*/
}   // namespace io
