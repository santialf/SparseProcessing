#pragma once

#include "coo.hpp"
#include "csc.hpp"
#include "csr.hpp"

namespace mtx::convert {

template <typename ValueType>
CSR<ValueType> COOToCSR(const COO<ValueType> &);

template <typename ValueType>
CSC<ValueType> COOToCSC(const COO<ValueType> &);

template <typename ValueType>
COO<ValueType> CSRToCOO(const CSR<ValueType> &);

template <typename ValueType>
COO<ValueType> CSCToCOO(const CSC<ValueType> &);

}  // namespace mtx::convert

#ifdef _HEADER_ONLY
#include "convertFormats.cpp"
#endif
