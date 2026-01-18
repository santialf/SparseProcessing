#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <type_traits>

#include "mtxReader.hpp"

bool readMtxLine(FILE* f, const MtxValueType& mtx_type, size_t& row, size_t& col, std::monostate& val)
{
    return fscanf(f, "%zu %zu", &row, &col) == 2;
}

bool readMtxLine(FILE* f, const MtxValueType& mtx_type, size_t& row, size_t& col, std::complex<double>& val)
{
    double real, imag;
    if (fscanf(f, "%zu %zu %lf %lf", &row, &col, &real, &imag) == 4)
        return false;

    val = std::complex<double>(real, imag);
    return true;
}

bool readMtxLine(FILE* f, const MtxValueType& mtx_type, size_t& row, size_t& col, int& val)
{
    return fscanf(f, "%zu %zu %d", &row, &col, &val) == 3;
}

bool readMtxLine(FILE* f, const MtxValueType& mtx_type, size_t& row, size_t& col, double& val)
{
    return fscanf(f, "%zu %zu %lf", &row, &col, &val) == 3;
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

template<typename valueType>
COO<valueType> readMtx(FILE* f, const MtxStructure& mtx, int num_rows, int num_cols) 
{
    COO<valueType> coo(num_rows, num_cols);
    size_t row, col;
    valueType val;

    while (readMtxLine(f, mtx.valueType, row, col, val))
    {
        coo.add_entry(row - 1, col - 1, val);

        if (mtx.symmetryType == MtxSymmetry::symmetric && row != col)
        {
            coo.add_entry(col - 1, row - 1, val);
        }
    }

    return coo;
}

template<typename valueType>
COO<valueType> readCoo(FILE* f)
{
    MM_typecode matcode;
    if (mm_read_banner(f, &matcode))
    {
        std::cerr << "Could not process Matrix Market banner.\n";
        exit(1);
    }

    auto [num_rows, num_cols] = validateMtx(f, matcode);

    MtxStructure mtx;
    if (mm_is_symmetric(matcode))
    {
        mtx.symmetryType = MtxSymmetry::symmetric;
    }

    if (mm_is_pattern(matcode)) 
    {
        mtx.valueType = MtxValueType::Pattern;
    } 
    else if (mm_is_complex(matcode)) 
    {
        mtx.valueType = MtxValueType::Complex;
    }
    else if (mm_is_integer(matcode))
    {
        mtx.valueType = MtxValueType::Integer;
    }
    else if (mm_is_real(matcode)) 
    {
        mtx.valueType = MtxValueType::Real;
    }

    return readMtx<valueType>(f, mtx, num_rows, num_cols);
}
