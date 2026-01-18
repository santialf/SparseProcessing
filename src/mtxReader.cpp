#include "mtxReader.hpp"

std::tuple<int, int, MtxSymmetry> validateMtx(FILE* f) {
   
    MM_typecode matcode;
    if (mm_read_banner(f, &matcode))
    {
        std::cerr << "Could not process Matrix Market banner.\n";
        exit(1);
    }
    
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

    MtxSymmetry symmetryType;
    if (mm_is_symmetric(matcode))
    {
        symmetryType = MtxSymmetry::symmetric;
    } 
    else 
    {
        symmetryType = MtxSymmetry::unsymmetric;
    }

    return {num_rows, num_cols, symmetryType};
}

bool readMtxLine(FILE* f, size_t& row, size_t& col, std::monostate& val)
{
    return fscanf(f, "%zu %zu", &row, &col) == 2;
}

bool readMtxLine(FILE* f, size_t& row, size_t& col, std::complex<double>& val)
{
    double real, imag;
    if (fscanf(f, "%zu %zu %lf %lf", &row, &col, &real, &imag) == 4)
        return false;

    val = std::complex<double>(real, imag);
    return true;
}

bool readMtxLine(FILE* f, size_t& row, size_t& col, int& val)
{
    return fscanf(f, "%zu %zu %d", &row, &col, &val) == 3;
}

bool readMtxLine(FILE* f, size_t& row, size_t& col, double& val)
{
    return fscanf(f, "%zu %zu %lf", &row, &col, &val) == 3;
}

template<typename valueType>
COO<valueType> readMtx(FILE* f, MtxSymmetry symmetry, int num_rows, int num_cols)
{
    COO<valueType> coo(num_rows, num_cols);
    size_t row, col;
    valueType val;

    while (readMtxLine(f, row, col, val))
    {
        coo.add_entry(row - 1, col - 1, val);

        if (symmetry == MtxSymmetry::symmetric && row != col)
        {
            coo.add_entry(col - 1, row - 1, val);
        }
    }

    return coo;
}

template<typename valueType>
COO<valueType> readCoo(FILE* f)
{
    auto [num_rows, num_cols, symmetryType] = validateMtx(f);

    return readMtx<valueType>(f, symmetryType, num_rows, num_cols);
}
