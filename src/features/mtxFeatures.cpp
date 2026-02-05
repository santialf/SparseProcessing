#include "mtxFeatures.hpp"

#include <algorithm>
#include <cmath>

namespace mtx::features {

namespace detail {

template <typename FormatType>
double totalEntries(const FormatType& mtx) {
  return static_cast<double>(mtx.nrows()) * static_cast<double>(mtx.ncols());
}

}  // namespace detail

template <typename FormatType>
double density(const FormatType& mtx) {
  return static_cast<double>(mtx.nnzs()) / (detail::totalEntries(mtx));
}

template <typename FormatType>
double sparsity(const FormatType& mtx) {
  return 1 - density(mtx);
}

template <typename IndexType, typename ValueType>
double nnzsPerRowMean(const CSR<IndexType, ValueType>& csr,
                      const bool exclude_empty_rows) {
  double row_mean = 0;
  IndexType non_empty_rows = 0;

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs = csr.rowPtr()[i + 1] - csr.rowPtr()[i];
    if (nnzs == 0) continue;
    row_mean += nnzs;
    non_empty_rows++;
  }

  if (exclude_empty_rows) {
    row_mean = row_mean / non_empty_rows;
  } else {
    row_mean = row_mean / csr.nrows();
  }

  return row_mean;
}

template <typename IndexType, typename ValueType>
double nnzsPerRowStandardDeviation(const CSR<IndexType, ValueType>& csr,
                                   const bool exclude_empty_rows, double mean) {
  if (mean == -1) mean = nnzsPerRowMean(csr, exclude_empty_rows);
  double row_standard_deviation = 0;
  IndexType non_empty_rows = 0;

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs = csr.rowPtr()[i + 1] - csr.rowPtr()[i];
    if (nnzs == 0) continue;
    row_standard_deviation += std::pow(nnzs - mean, 2.0);
    non_empty_rows++;
  }

  if (exclude_empty_rows) {
    row_standard_deviation = row_standard_deviation / non_empty_rows;
  } else {
    row_standard_deviation = row_standard_deviation / csr.nrows();
  }
  row_standard_deviation = std::sqrt(row_standard_deviation);

  return row_standard_deviation;
}

template <typename IndexType, typename ValueType>
double rowCoefficientOfVariation(const CSR<IndexType, ValueType>& csr,
                                 const bool exclude_empty_rows) {
  auto row_mean = nnzsPerRowMean(csr, exclude_empty_rows);
  auto row_std = nnzsPerRowStandardDeviation(csr, exclude_empty_rows, row_mean);
  double imbalance_factor{static_cast<double>(row_std) /
                          static_cast<double>(row_mean)};
  return imbalance_factor;
}

template <typename IndexType, typename ValueType>
IndexType maxNnzsInRow(const CSR<IndexType, ValueType>& csr) {
  IndexType max = 0;
  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs = csr.rowPtr()[i + 1] - csr.rowPtr()[i];
    max = std::max(nnzs, max);
  }
  return max;
}

// max_nnzs_per_row
// min_nnzs_per_row

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