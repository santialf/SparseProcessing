#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <variant>
#include <complex>
#include <filesystem>
#include <fstream>
#include <cassert>

#include "coo.hpp"

namespace mtx::io {

enum class MtxSymmetry
{
    general,
    symmetric,
    skewed,
    hermitian
};

enum class MtxValueType
{
    pattern,
    integer,
    real,
    complex
};

enum class MtxStorage {
    sparse,
    dense
};

struct MtxStructure
{
    size_t num_rows;
    size_t num_cols;
    size_t num_entries;
    size_t num_nnzs;
    MtxSymmetry symmetry;
    MtxValueType type;
    MtxStorage storage;
};

void parseMtxStorage(MtxStructure&, const std::string);
void parseMtxSymmetry(MtxStructure&, const std::string); 
void parseMtxType(MtxStructure&, const std::string);
MtxStructure parseMtx(std::ifstream&); 

bool readCOOLine(std::ifstream&, size_t&, size_t&);
bool readCOOLine(std::ifstream&, size_t&, size_t&, std::complex<double>&);
template<typename valueType>
bool readCOOLine(std::ifstream&, size_t&, size_t&, valueType&);
template<typename valueType>
COO<valueType> readCOO(std::ifstream&, const MtxStructure&);
template<typename valueType>
COO<valueType> readMtxToCOO(const std::filesystem::path&);

template<typename valueType>
size_t countNnzs(std::ifstream&, const MtxStructure&);

std::ifstream openFile(const std::filesystem::path&);

} //namespace mtx::io

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif

