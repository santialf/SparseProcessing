#include "mtxReader.hpp"

namespace mtx::io {

void parseMtxStorage(MtxStructure& mtx, const std::string storage) 
{
    if (storage == "coordinate")
    {
        mtx.storage = MtxStorage::sparse;
    }
    else if (storage == "array")
    {
        mtx.storage = MtxStorage::dense;
    }
    else 
    {
        throw std::runtime_error("Unsupported mtx storage");
    }
}

void parseMtxSymmetry(MtxStructure& mtx, const std::string symmetry) 
{
    if (symmetry == "general")
    {
        mtx.symmetry = MtxSymmetry::general;
    } 
    else if (symmetry == "symmetric")
    {
        mtx.symmetry = MtxSymmetry::symmetric;
    }
    else if (symmetry == "skew-symmetric")
    {
        mtx.symmetry = MtxSymmetry::skewed;
    }
    else if (symmetry == "hermitian")
    {
        mtx.symmetry = MtxSymmetry::hermitian;
    }
    else 
    {
        throw std::runtime_error("Unsupported mtx symmetry");
    }
}

void parseMtxType(MtxStructure& mtx, const std::string type) 
{
    if (type == "real")
    {
        mtx.type = MtxValueType::real;
    }
    else if (type == "integer")
    {
        mtx.type = MtxValueType::integer;
    }
    else if (type == "pattern")
    {
        mtx.type = MtxValueType::pattern;
    }
    else if (type == "complex")
    {
        mtx.type = MtxValueType::complex;
    }
    else
    {
        throw std::runtime_error("Unsupported mtx type");
    }
}

void skipMtxCommentLines(std::ifstream& file)
{
    while (file.peek() == '%')
    {
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void parseMtxHeader(std::ifstream& file, MtxStructure& mtx)
{
    std::string banner, object, storage, type, symmetry;
    file >> banner >> object >> storage >> type >> symmetry;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if ((banner != "%%MatrixMarket") || (object != "matrix"))
    {
        throw std::runtime_error("Invalid MTX banner or unsupported mtx object");
    }

    parseMtxStorage(mtx, storage);
    parseMtxType(mtx, type);
    parseMtxSymmetry(mtx, symmetry);
}

void parseMtxSize(std::ifstream& file, MtxStructure& mtx)
{
    file >> mtx.num_rows >> mtx.num_cols >> mtx.num_entries;
}

MtxStructure parseMtx(std::ifstream& file) 
{
    MtxStructure mtx;
    parseMtxHeader(file, mtx);
    skipMtxCommentLines(file);
    parseMtxSize(file, mtx);

    return mtx;
}

bool readCOOLine(std::ifstream& file, size_t& row, size_t& col)
{
    if (!(file >> row >> col))
    {
        return false;
    }
    return true;
}

bool readCOOLine(std::ifstream& file, size_t& row, size_t& col, std::complex<double>& val)
{
    double real, imag;
    if (!(file >> row >> col >> real >> imag))
    {
        return false;
    }
    val = {real, imag};
    return true;
}

template<typename valueType>
bool readCOOLine(std::ifstream& file, size_t& row, size_t& col, valueType& val)
{
    if (!(file >> row >> col >> val))
    {
        return false;
    }
    return true;
}

template<typename valueType>
COO<valueType> readCOO(std::ifstream& file, const MtxStructure& mtx)
{
    COO<valueType> coo(mtx.num_rows, mtx.num_cols);
    size_t row, col;
    valueType val = static_cast<valueType>(1);

    if (mtx.type == MtxValueType::pattern)
    {
        while (readCOOLine(file, row, col))
        {
            coo.add_entry(row - 1, col - 1, val);

            if (mtx.symmetry == MtxSymmetry::symmetric && row != col)
            {
                coo.add_entry(col - 1, row - 1, val);
            }
        }
    }
    else
    {
        while (readCOOLine(file, row, col, val))
        {
            coo.add_entry(row - 1, col - 1, val);

            if (mtx.symmetry == MtxSymmetry::symmetric && row != col)
            {
                coo.add_entry(col - 1, row - 1, val);
            }
        }
    }
    
    return coo;
}

template<typename valueType>
COO<valueType> readMtxToCOO(const std::filesystem::path path)
{
    std::ifstream file {path};
    if (!file) 
    {
        throw std::runtime_error("Bad matrix file name or path: " + path.string());
    }

    auto mtx = parseMtx(file);

    return readCOO<valueType>(file, mtx);
}

} //namespace mtx::io
