#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

struct COO
{
    int row;
    int col;
    double val;
};

void printCOO(const std::vector<COO>& entries);
void sortCOO(std::vector<COO>& entries);
