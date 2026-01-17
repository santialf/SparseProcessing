#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <type_traits>

#include "mtxReader.hpp"

template<typename valueType>
bool readMtxLine(FILE* f, MtxValueType mtx_type, size_t& row, size_t& col, valueType& val)
{
    bool success = false;
    if (mtx_type == MtxValueType::Pattern) 
    {
        success = fscanf(f, "%zu %zu", &row, &col) == 2;
    } 
    else if (mtx_type == MtxValueType::Complex) 
    {
        // TO BE IMPLEMENTED
        //double real, imag;
        //success = fscanf(f, "%zu %zu %lf %lf", &row, &col, &real, &imag) == 4;
        //if constexpr (is_complex<valueType>::value)
        //{
          //  val = valueType(real, imag);
        //}
    }
    else if (mtx_type == MtxValueType::Integer)
    {
        success = fscanf(f, "%zu %zu %d", &row, &col, &val) == 3;
    }
    else if (mtx_type == MtxValueType::Real)
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

template<typename valueType>
COO<valueType> readMtx(FILE* f, MtxStructure mtx, int num_rows, int num_cols) 
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
