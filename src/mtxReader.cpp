#include <iostream>
#include <vector>
#include <algorithm>

#include "mtxReader.hpp"
#include "coo.hpp"

extern "C" 
{
#include "mmio.h"
}

COO readMtxLine(FILE* f, MM_typecode matcode)
{
    int row, col;
    double val;
    if (mm_is_pattern(matcode)) 
    {
        if (fscanf(f, "%d %d", &row, &col) != 2) 
        {
            std::cerr << "Error reading MTX entry at index\n";
            exit(1);
        }
        val = 1;
    } 
    else if (mm_is_complex(matcode)) 
    {
        // TO BE IMPLEMENTED
        std::cerr << "Matrix with complex numbers to be implemented...\n";
        exit(1);
    }
    else 
    {
        if (fscanf(f, "%d %d %lf", &row, &col, &val) != 3) 
        {
            std::cerr << "Error reading MTX entry at index\n";
            exit(1);
        }
    }
    return COO{row - 1, col - 1, val};
}

void readMtxCoordinates(FILE* f, MM_typecode matcode, std::vector<COO>& entries, matrixStructure& mtx)
{
    for (int i = 0; i < mtx.entries; i++) 
    {
        COO coordinate = readMtxLine(f, matcode);
        entries.push_back(coordinate);

        if (mm_is_symmetric(matcode) && coordinate.row != coordinate.col)
        {
            entries.push_back({coordinate.col, coordinate.row, coordinate.val});
            mtx.nz++;
        }
        mtx.nz++;
    }
}
