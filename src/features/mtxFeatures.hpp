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

/* template <typename IndexType, typename ValueType>
double nnzsPerRowStandardDeviation(const CSR<IndexType, ValueType>&,
                                   const bool = false); */
/*
template <typename IndexType, typename ValueType>
double rowImbalanceFactor(const CSR<IndexType, ValueType>&, const bool = false);
*/

}  // namespace mtx::features

#ifdef _HEADER_ONLY
#include "mtxFeatures.cpp"
#endif
