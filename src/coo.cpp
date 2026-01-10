#include <iostream>
#include <vector>
#include <algorithm>

#include "coo.hpp"

extern "C" 
{
#include "mmio.h"
}

void printCOO(const std::vector<COO>& entries)
{
    for (const auto& e : entries) 
    {
        std::cout << e.row << " "
                  << e.col << " "
                  << e.val
                  << "\n";
    }
}

void sortCOO(std::vector<COO>& entries)
{
    std::sort(entries.begin(), entries.end(),
        [](const COO& a, const COO& b) 
        {
            if (a.row != b.row) 
            {
                return a.row < b.row;
            }
            return a.col < b.col;
        });
}