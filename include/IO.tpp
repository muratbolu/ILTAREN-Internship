#pragma once

#include "Chr.tpp"
#include "LinkedList.tpp"
#include "NodeIter.tpp"

#include <cstdio>

namespace io
{
// Prints the numbers as unsigned (%u)
void print(FILE* stream, unsigned i) noexcept
{
    fprintf(stream, "%u", i);
}

void print(FILE* stream, const chr::Time& t) noexcept
{
    fprintf(stream, "%02hhu:%02hhu", t.getHour(), t.getMinute());
}

void print(FILE* stream, const chr::Duration& d) noexcept
{
    fprintf(stream, "%u", d.getDuration());
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
