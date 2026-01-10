#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

extern "C" 
{
#include "mmio.h"
}

struct matrixStructure 
{
    int num_rows;
    int num_cols;
    int entries;
    int nz = 0;
};

struct COO
{
    int row;
    int col;
    double val;
};

COO readMtxLine(FILE* f, MM_typecode matcode);
void readMtxCoordinates(FILE* f, MM_typecode matcode, std::vector<COO>& entries, matrixStructure& mtx);
void printCOO(const std::vector<COO>& entries);
void sortCOO(std::vector<COO>& entries);
