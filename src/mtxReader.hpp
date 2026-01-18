#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <variant>
#include <complex>

#include "coo.hpp"

extern "C" 
{
#include "mmio.h"
}

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
    MtxSymmetry symmetry;
    MtxValueType type;
    MtxStorage storage;
};

void parseMtxStorage(MtxStructure&, MM_typecode);
void parseMtxSymmetry(MtxStructure&, MM_typecode); 
void parseMtxType(MtxStructure&, MM_typecode);
MtxStructure parseMtx(FILE*); 

bool readMtxLine(FILE*, size_t&, size_t&, std::monostate&);
bool readMtxLine(FILE*, size_t&, size_t&, std::complex<double>&);
bool readMtxLine(FILE*, size_t&, size_t&, int&);
bool readMtxLine(FILE*, size_t&, size_t&, double&);

template<typename valueType>
COO<valueType> readMtx(FILE*, const MtxStructure&);

template<typename valueType>
COO<valueType> readCoo(FILE*);

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif

