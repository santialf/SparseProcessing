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
    symmetric,
    unsymmetric
};

template<typename valueType>
bool readMtxLine(FILE*, size_t&, size_t&, valueType&);

std::tuple<int, int, MtxSymmetry> validateMtx(FILE*); 

template<typename valueType>
COO<valueType> readMtx(FILE*, MtxSymmetry symmetry, int, int);

template<typename valueType>
COO<valueType> readCoo(FILE*);

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif

