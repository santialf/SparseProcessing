#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "mtxReader.hpp"

bool readMtxLine(FILE* f, MM_typecode matcode, size_t& row, size_t& col, double& val)
{
    bool success = false;
    if (mm_is_pattern(matcode)) 
    {
        success = fscanf(f, "%zu %zu", &row, &col) == 2;
    } 
    else if (mm_is_complex(matcode)) 
    {
        // TO BE IMPLEMENTED
        std::cerr << "Matrix with complex numbers to be implemented...\n";
        exit(1);
    }
    else 
    {
        success = fscanf(f, "%zu %zu %lf", &row, &col, &val) == 3;
    }
    return success;
}

void readMtxCoordinates(FILE* f, MM_typecode matcode, COO& coo)
{
    size_t row, col;
    double val = 1.0;
    while (readMtxLine(f, matcode, row, col, val))
    {
        coo.add_entry(row - 1, col - 1, val);

        if (mm_is_symmetric(matcode) && row != col)
        {
            coo.add_entry(col - 1, row - 1, val);
        }
    }
}
