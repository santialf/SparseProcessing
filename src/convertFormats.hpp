#pragma once

#include "bell.hpp"
#include "coo.hpp"
#include "csc.hpp"
#include "csr.hpp"

namespace mtx::convert {

template <typename ValueType>
CSR<ValueType> COOToCSR(const COO<ValueType> &);

template <typename ValueType>
CSC<ValueType> COOToCSC(const COO<ValueType> &);

template <typename ValueType>
size_t findEllCols(const COO<ValueType> &, const size_t);
template <typename ValueType>
std::unique_ptr<size_t[]> findColBlockIdx(const COO<ValueType> &, const size_t,
                                          const size_t, const size_t);
template <typename ValueType>
std::unique_ptr<ValueType[]> findVals(const COO<ValueType> &, const size_t,
                                      const size_t, const size_t,
                                      const std::unique_ptr<size_t[]> &);
template <typename ValueType>
BELL<ValueType> COOToBELL(const COO<ValueType> &, size_t);

template <typename ValueType>
COO<ValueType> CSRToCOO(const CSR<ValueType> &);

template <typename ValueType>
COO<ValueType> CSCToCOO(const CSC<ValueType> &);

}  // namespace mtx::convert

#ifdef _HEADER_ONLY
#include "convertFormats.cpp"
#endif
