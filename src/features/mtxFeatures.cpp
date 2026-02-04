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