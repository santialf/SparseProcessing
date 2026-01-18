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

enum class MtxValueType 
{
    Pattern,
    Real,
    Integer,
    Complex
};

enum class MtxSymmetry
{
    symmetric,
    unsymmetric
};

struct MtxStructure {
    MtxValueType valueType;
    MtxSymmetry symmetryType;
};

template<typename valueType>
bool readMtxLine(FILE*, const MtxStructure&, size_t&, size_t&, valueType&);

std::tuple<int, int> validateMtx(FILE*, MM_typecode); 

template<typename valueType>
COO<valueType> readMtx(FILE*, const MtxStructure&, int, int);

template<typename valueType>
COO<valueType> readCoo(FILE*);

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif

