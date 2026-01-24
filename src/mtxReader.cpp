#include "mtxReader.hpp"

namespace mtx::io {

std::string toLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

void parseMtxStorage(MtxStructure& mtx, std::string storage) 
{
    storage = toLower(storage);

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
        throw std::runtime_error(
            "Unsupported mtx storage '" + storage +
            "'. Expected: coordinate | array"
        );
    }
}

void parseMtxSymmetry(MtxStructure& mtx, std::string symmetry) 
{
    symmetry = toLower(symmetry);

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
        throw std::runtime_error(
            "Unsupported mtx symmetry '" + symmetry +
            "'. Expected: general | symmetric | skew-symmetric | hermitian"
        );
    }
}

void parseMtxType(MtxStructure& mtx, std::string type) 
{
    type = toLower(type);

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
        throw std::runtime_error(
            "Unsupported mtx value type '" + type +
            "'. Expected: real | integer | pattern | complex"
        );
    }
}

void skipMtxCommentLines(std::ifstream& file)
{
    while (file && file.peek() == '%')
    {
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void parseMtxHeader(std::ifstream& file, MtxStructure& mtx)
{
    std::string banner, object, storage, type, symmetry;
    if (!(file >> banner >> object >> storage >> type >> symmetry))
    {
        throw std::runtime_error("Failed to read mtx header");
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (banner != "%%MatrixMarket" || object != "matrix")
    {
        throw std::runtime_error(
            "Invalid Matrix Market header. "
            "Expected banner '%%MatrixMarket matrix', but got '" +
            banner + " " + object + "'"
        );
    }

    parseMtxStorage(mtx, storage);
    parseMtxType(mtx, type);
    parseMtxSymmetry(mtx, symmetry);
}

void parseMtxSize(std::ifstream& file, MtxStructure& mtx)
{
    if (!(file >> mtx.num_rows >> mtx.num_cols >> mtx.num_entries)) 
    {
        throw std::runtime_error("Failed to read matrix dimensions");
    }
    if (mtx.num_rows <= 0 || mtx.num_cols <= 0 || mtx.num_entries <= 0) 
    {
        throw std::runtime_error("Matrix dimensions must be positive");
    }
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

    size_t ctr{0};
    for (size_t i = 0; i < mtx.num_entries; i++)
    {
        bool ok = (mtx.type == MtxValueType::pattern)
                  ? readCOOLine(file, row, col)
                  : readCOOLine(file, row, col, val);

        if (!ok)
        {
            throw std::runtime_error(
                "Failed to read mtx entry at data line "
                + std::to_string(i + 1)
            );
        }

        if (row == 0 || row > mtx.num_rows || col == 0 || col > mtx.num_cols)
        {
            throw std::runtime_error(
                "Out of bounds entry at data line "
                + std::to_string(i + 1)
            );
        }
            
        rowIdx[ctr] = row - 1;
        colIdx[ctr] = col - 1;
        vals[ctr] = val;
        ctr++;
        if (mtx.symmetry != MtxSymmetry::general && row != col)
        {
            rowIdx[ctr] = col - 1;
            colIdx[ctr] = row - 1;
            vals[ctr] = val;
            ctr++;
        }
    }
    assert(ctr == mtx.num_nnzs);

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
        bool ok = (mtx.type == MtxValueType::pattern)
                  ? readCOOLine(file, row, col)
                  : readCOOLine(file, row, col, val);

        if (!ok)
        {
            throw std::runtime_error(
                "Failed to read mtx entry at data line "
                + std::to_string(i + 1)
            );
        }
            
        nnzs++;
        if (row != col)
        {
            nnzs++;
        }
    }
    
    return nnzs;
}

std::ifstream openFile(const std::filesystem::path& path)
{
    std::ifstream file {path};
    if (!file) 
    {
        throw std::runtime_error("Failed to open Matrix Market file: " + path.string());
    }
    return file;
}

template<typename valueType>
COO<valueType> readMtxToCOO(const std::filesystem::path& path)
{
    std::ifstream file = openFile(path);

    auto mtx = parseMtx(file);
    std::streampos data_pos = file.tellg(); 

    if (mtx.symmetry == MtxSymmetry::general)
    {
        mtx.num_nnzs = mtx.num_entries;
    }
    else
    {
        mtx.num_nnzs = countNnzs<valueType>(file, mtx);
    }

    file.seekg(data_pos);  
    return readCOO<valueType>(file, mtx);
}

} //namespace mtx::io
