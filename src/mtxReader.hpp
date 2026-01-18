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

bool readMtxLine(FILE*, size_t&, size_t&, std::monostate&);
bool readMtxLine(FILE*, size_t&, size_t&, std::complex<double>&);
bool readMtxLine(FILE*, size_t&, size_t&, int&);
bool readMtxLine(FILE*, size_t&, size_t&, double&);

std::tuple<int, int, MtxSymmetry> validateMtx(FILE*); 

template<typename valueType>
COO<valueType> readMtx(FILE*, MtxSymmetry symmetry, int, int);

template<typename valueType>
COO<valueType> readCoo(FILE*);

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif

