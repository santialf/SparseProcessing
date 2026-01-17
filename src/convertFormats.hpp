#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include <coo.hpp>
#include <csr.hpp>

template<typename valueType>
CSR<valueType> convertCOOToCSR(const COO<valueType>&);

#ifdef _HEADER_ONLY
#include "convertFormats.cpp"
#endif
