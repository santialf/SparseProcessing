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

bool readMtxLine(FILE*, MM_typecode, size_t&, size_t&, double&);
void readMtxCoordinates(FILE*, MM_typecode, COO&);
