#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "coo.hpp"

extern "C" 
{
#include "mmio.h"
}

bool readMtxLine(FILE*, MM_typecode, size_t&, size_t&, double&);
std::tuple<int, int> validateMtx(FILE*, MM_typecode); 

template<typename valueType>
COO<valueType> readMtx(FILE*, MM_typecode, int, int);

template<typename valueType>
COO<valueType> readCoo(FILE*);

#ifdef _HEADER_ONLY
#include "mtxReader.cpp"
#endif

