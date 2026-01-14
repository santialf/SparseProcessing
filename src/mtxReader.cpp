#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "mtxReader.hpp"

std::tuple<int, int, double> readMtxLine(FILE* f, MM_typecode matcode)
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
    return {row - 1, col - 1, val};
}

void readMtxCoordinates(FILE* f, MM_typecode matcode, COO& entries, matrixStructure& mtx)
{
    for (int i = 0; i < mtx.entries; i++) 
    {
        auto [row, col, val] = readMtxLine(f, matcode);
        entries.row_indices.push_back(row);
        entries.col_indices.push_back(col);
        entries.values.push_back(val);

        if (mm_is_symmetric(matcode) && row != col)
        {
            entries.row_indices.push_back(row);
            entries.col_indices.push_back(col);
            entries.values.push_back(val);
            mtx.nz++;
        }
        mtx.nz++;
    }
}
