#pragma once

#include <vector>
#include <algorithm>

#include <coo.hpp>
#include <csr.hpp>
#include <csc.hpp>

namespace mtx::convert {

template<typename valueType>
CSR<valueType> COOToCSR(const COO<valueType>&);

template<typename valueType>
CSC<valueType> COOToCSC(const COO<valueType>&);

} // namespace mtx::convert

#ifdef _HEADER_ONLY
#include "convertFormats.cpp"
#endif
