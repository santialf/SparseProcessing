#pragma once

#include "formats/bell.hpp"
#include "formats/coo.hpp"
#include "formats/csc.hpp"
#include "formats/csr.hpp"

namespace mtx::convert {

template <typename IndexType, typename ValueType>
CSR<IndexType, ValueType> COOToCSR(const COO<IndexType, ValueType> &);

template <typename IndexType, typename ValueType>
CSC<IndexType, ValueType> COOToCSC(const COO<IndexType, ValueType> &);

template <typename IndexType, typename ValueType>
IndexType findEllCols(const COO<IndexType, ValueType> &, const IndexType);
template <typename IndexType, typename ValueType>
std::unique_ptr<int[]> findColBlockIdx(const COO<IndexType, ValueType> &,
                                       const IndexType, const IndexType,
                                       const IndexType);
template <typename IndexType, typename ValueType>
std::unique_ptr<ValueType[]> findVals(const COO<IndexType, ValueType> &,
                                      const IndexType, const IndexType,
                                      const IndexType,
                                      const std::unique_ptr<int[]> &);
template <typename IndexType, typename ValueType>
BELL<IndexType, ValueType> COOToBELL(const COO<IndexType, ValueType> &,
                                     IndexType);

template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> CSRToCOO(const CSR<IndexType, ValueType> &);

template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> CSCToCOO(const CSC<IndexType, ValueType> &);

}  // namespace mtx::convert

#ifdef _HEADER_ONLY
#include "convertFormats.cpp"
#endif
