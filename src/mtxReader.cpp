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

std::tuple<int, int> validateMtx(FILE* f, MM_typecode matcode) {
    
    if (!mm_is_matrix(matcode) || !mm_is_coordinate(matcode)) {
        std::cerr << "Input must be a sparse Matrix Market matrix (coordinate format).\n";
        exit(1);
    }

    int num_rows, num_cols, num_entries;
    if (mm_read_mtx_crd_size(f, &num_rows, &num_cols, &num_entries))
    {
        std::cerr << "Could not read matrix dimensions.\n";
        exit(1);
    }
    return {num_rows, num_cols};
}

COO readMtx(FILE* f, MM_typecode matcode, int num_rows, int num_cols) 
{
    COO coo(num_rows, num_cols);
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

    return coo;
}

COO readCoo(FILE* f)
{
    MM_typecode matcode;
    if (mm_read_banner(f, &matcode))
    {
        std::cerr << "Could not process Matrix Market banner.\n";
        exit(1);
    }

    auto [num_rows, num_cols] = validateMtx(f, matcode);
    
    return readMtx(f, matcode, num_rows, num_cols);
}
