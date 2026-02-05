#include "mtxFeatures.hpp"

namespace mtx::features {

namespace detail {

template <typename FormatType>
double totalEntries(const FormatType& mtx) {
  return static_cast<double>(mtx.nrows()) * static_cast<double>(mtx.ncols());
}

}  // namespace detail

template <typename FormatType>
double density(const FormatType& mtx) {
  return static_cast<double>(mtx.nnz()) / (detail::totalEntries(mtx));
}

template <typename FormatType>
double sparsity(const FormatType& mtx) {
  return 1 - density(mtx);
}

template <typename IndexType, typename ValueType>
double nnzsPerRowMean(const CSR<IndexType, ValueType>& csr,
                      const bool excludeEmptyRows) {
  double rowMean = 0;
  IndexType nonEmptyRows = 0;

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs = csr.rowPtr()[i + 1] - csr.rowPtr()[i];
    if (nnzs == 0) continue;
    rowMean += nnzs;
    nonEmptyRows++;
  }
  if (excludeEmptyRows) {
    rowMean = rowMean / nonEmptyRows;
  } else {
    rowMean = rowMean / csr.nrows();
  }

  return rowMean;
}

/* template <typename IndexType, typename ValueType>
double nnzsPerRowStandardDeviation(const CSR<IndexType, ValueType>& csr,
                                   const bool excludeEmptyRows,
                                   const double mean) {
  if (mean == -1) mean = nnzsPerRowMean(csr, excludeEmptyRows);

  double rowStandardDeviation = 0;
  return rowStandardDeviation;
} */

/* template <typename IndexType, typename ValueType>
double rowImbalanceFactor(const CSR<IndexType, ValueType>& csr,
                          const bool excludeEmptyRows) {
  auto rowMean = nnzsPerRowMean(csr, excludeEmptyRows);
  auto rowStd = nnzsPerRowStandardDeviation(csr, excludeEmptyRows);
  double imbalanceFactor =
      static_cast<double>(rowStd) / static_cast<double>(rowMean);
  return imbalanceFactor;
} */

// nnz_per_row_mean
// nnz_per_row_std
// nnz_per_col_mean
// nnz_per_col_std
// max_nnz_per_row
// min_nnz_per_row

// mtx bandwidth
// average row bandwidth
// symmmetry
// diagonal density
// number of empty rows
// number of empty columns

// ELL padding
// row imbalance
// column imbalance

}  // namespace mtx::features