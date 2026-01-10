#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

extern "C" 
{
#include "mmio.h"
}

struct COO
{
    int row;
    int col;
    double val;
};

void printCOO(const std::vector<COO>& entries);
void sortCOO(std::vector<COO>& entries);
