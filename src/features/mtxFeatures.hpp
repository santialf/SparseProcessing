#pragma once

#include "formats/bell.hpp"
#include "formats/coo.hpp"
#include "formats/csc.hpp"
#include "formats/csr.hpp"

namespace mtx::features {

namespace detail {

template <typename FormatType>
double totalEntries(const FormatType&);

}  // namespace detail

template <typename FormatType>
double density(const FormatType&);

template <typename FormatType>
double sparsity(const FormatType&);

template <typename IndexType, typename ValueType>
double nnzsPerRowMean(const CSR<IndexType, ValueType>&, const bool = false);

template <typename IndexType, typename ValueType>
double nnzsPerRowStandardDeviation(const CSR<IndexType, ValueType>&,
                                   const bool = false, double = -1);

template <typename IndexType, typename ValueType>
double rowCoefficientOfVariation(const CSR<IndexType, ValueType>&,
                                 const bool = false);

template <typename IndexType, typename ValueType>
IndexType maxNnzsInRow(const CSR<IndexType, ValueType>&);

template <typename IndexType, typename ValueType>
double bandwidth(const CSR<IndexType, ValueType>&);

template <typename IndexType, typename ValueType>
bool isSymmetric(const CSR<IndexType, ValueType>&);

template <typename IndexType, typename ValueType>
IndexType numEmptyRows(const CSR<IndexType, ValueType>&);

template <typename IndexType, typename ValueType>
IndexType numEmptyCols(const CSR<IndexType, ValueType>&);

}  // namespace mtx::features

#ifdef _HEADER_ONLY
#include "mtxFeatures.cpp"
#endif
