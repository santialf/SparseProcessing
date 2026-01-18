#include "mtxReader.hpp"

void parseMtxStorage(MtxStructure& mtx, MM_typecode matcode) 
{
    if (mm_is_coordinate(matcode))
    {
        mtx.storage = MtxStorage::sparse;
    }
    else if (mm_is_array(matcode))
    {
        mtx.storage = MtxStorage::dense;
    }
}

void parseMtxSymmetry(MtxStructure& mtx, MM_typecode matcode) 
{
    if (mm_is_general(matcode))
    {
        mtx.symmetry = MtxSymmetry::general;
    } 
    else if (mm_is_symmetric(matcode))
    {
        mtx.symmetry = MtxSymmetry::symmetric;
    }
    else if (mm_is_skew(matcode))
    {
        mtx.symmetry = MtxSymmetry::skewed;
    }
    else if (mm_is_hermitian(matcode))
    {
        mtx.symmetry = MtxSymmetry::hermitian;
    }
}

void parseMtxType(MtxStructure& mtx, MM_typecode matcode) 
{
    if (mm_is_real(matcode))
    {
        mtx.type = MtxValueType::real;
    }
    else if (mm_is_integer(matcode))
    {
        mtx.type = MtxValueType::integer;
    }
    else if (mm_is_pattern(matcode))
    {
        mtx.type = MtxValueType::pattern;
    }
    else if (mm_is_complex(matcode))
    {
        mtx.type = MtxValueType::complex;
    }
}

MtxStructure parseMtx(FILE* f) {
   
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

    MtxStructure mtx;
    mtx.num_rows = static_cast<size_t>(num_rows);
    mtx.num_cols = static_cast<size_t>(num_cols);
    mtx.num_entries = static_cast<size_t>(num_entries);
    
    parseMtxStorage(mtx, matcode);
    parseMtxSymmetry(mtx, matcode);
    parseMtxType(mtx, matcode);

    return mtx;
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
COO<valueType> readMtx(FILE* f, const MtxStructure& mtxStruct)
{
    COO<valueType> coo(mtxStruct.num_rows, mtxStruct.num_cols);
    size_t row, col;
    valueType val;

    while (readMtxLine(f, row, col, val))
    {
        coo.add_entry(row - 1, col - 1, val);

        if (mtxStruct.symmetry == MtxSymmetry::symmetric && row != col)
        {
            coo.add_entry(col - 1, row - 1, val);
        }
    }

    return coo;
}

template<typename valueType>
COO<valueType> readCoo(FILE* f)
{
    auto mtxStruct = parseMtx(f);

    return readMtx<valueType>(f, mtxStruct);
}
