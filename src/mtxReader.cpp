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
    size_t row, col;
    valueType val {static_cast<valueType>(1)};

    auto rowIdx = std::make_unique<size_t[]>(mtx.num_nnzs);
    auto colIdx = std::make_unique<size_t[]>(mtx.num_nnzs);
    auto vals   = std::make_unique<valueType[]>(mtx.num_nnzs);

    for (size_t i = 0; i < mtx.num_nnzs; i++)
    {
        if (mtx.type == MtxValueType::pattern)
        {
            readCOOLine(file, row, col);
        }
        else 
        {
            readCOOLine(file, row, col, val);
        }
            
        rowIdx[i] = row - 1;
        colIdx[i] = col - 1;
        vals[i] = val;
        if (mtx.symmetry == MtxSymmetry::symmetric && row != col)
        {
            i++;
            rowIdx[i] = col - 1;
            colIdx[i] = row - 1;
            vals[i] = val;
        }
    }

    return COO<valueType>(COO<valueType>::adopt, rowIdx.release(), colIdx.release(), vals.release(),
        mtx.num_rows, mtx.num_cols, mtx.num_nnzs);
}

template<typename valueType>
size_t countNnzs(std::ifstream& file, const MtxStructure& mtx)
{
    size_t nnzs{0};
    size_t row, col;
    valueType val {static_cast<valueType>(1)};

    for (size_t i = 0; i < mtx.num_entries; i++)
    {
        if (mtx.type == MtxValueType::pattern)
        {
            readCOOLine(file, row, col);
        }
        else 
        {
            readCOOLine(file, row, col, val);
        }
            
        nnzs++;
        if (row != col)
        {
            nnzs++;
        }
    }
    
    return nnzs;
}

std::ifstream openFile(const std::filesystem::path path)
{
    std::ifstream file {path};
    if (!file) 
    {
        throw std::runtime_error("Bad matrix file name or path: " + path.string());
    }
    return file;
}

template<typename valueType>
COO<valueType> readMtxToCOO(const std::filesystem::path path)
{
    std::ifstream file = openFile(path);

    auto mtx = parseMtx(file);
    std::streampos data_pos = file.tellg(); 

    if (mtx.symmetry == MtxSymmetry::symmetric)
    {
        mtx.num_nnzs = countNnzs<valueType>(file, mtx);
    }
    else
    {
        mtx.num_nnzs = mtx.num_entries;
    }

    file.seekg(data_pos);  
    return readCOO<valueType>(file, mtx);
}

} //namespace mtx::io
