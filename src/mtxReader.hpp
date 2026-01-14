#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "coo.hpp"

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

std::tuple<int, int, double> readMtxLine(FILE* f, MM_typecode matcode);
void readMtxCoordinates(FILE* f, MM_typecode matcode, COO& entries, matrixStructure& mtx);
