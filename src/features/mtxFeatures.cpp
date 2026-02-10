#include "mtxFeatures.hpp"

#include <algorithm>
#include <cmath>

#include "converter/convertFormats.hpp"

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
  double row_mean{0};
  IndexType non_empty_rows{0};

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs{csr.rowPtr()[i + 1] - csr.rowPtr()[i]};
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
  double row_standard_deviation{0};
  IndexType non_empty_rows{0};

  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs{csr.rowPtr()[i + 1] - csr.rowPtr()[i]};
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
  IndexType max{0};
  for (IndexType i = 0; i < csr.nrows(); ++i) {
    IndexType nnzs{csr.rowPtr()[i + 1] - csr.rowPtr()[i]};
    max = std::max(nnzs, max);
  }
  return max;
}

template <typename IndexType, typename ValueType>
double bandwidth(const CSR<IndexType, ValueType>& csr) {
  double bw = 0;
  for (int i = 0; i < csr.nrows(); i++) {
    for (int j = csr.rowPtr()[i]; j < csr.rowPtr()[i + 1]; j++) {
      // Compute the absolute distance from the diagonal
      IndexType dist =
          (i > csr.colIdx()[j]) ? (i - csr.colIdx()[j]) : (csr.colIdx()[j] - i);
      if (dist > bw) bw = dist;
    }
  }
  bw = bw / csr.nrows();
  return bw;
}

template <typename IndexType, typename ValueType>
bool isSymmetric(const CSR<IndexType, ValueType>& csr) {
  if (csr.nrows() != csr.ncols()) return false;

  for (IndexType i = 0; i < csr.nrows(); i++) {
    for (IndexType j = csr.rowPtr()[i]; j < csr.rowPtr()[i + 1]; j++) {
      IndexType col{csr.colIdx()[j]};
      ValueType val{csr.vals()[j]};

      bool found{false};
      for (IndexType jj = csr.rowPtr()[col]; jj < csr.rowPtr()[col + 1]; jj++) {
        if (csr.colIdx()[jj] == i) {
          if (csr.vals()[jj] != val) return false;
          found = true;
          break;
        }
      }

      if (!found) return false;
    }
  }
  return true;
}

template <typename IndexType, typename ValueType>
IndexType numEmptyRows(const CSR<IndexType, ValueType>& csr) {
  IndexType ctr = 0;

  for (int i = 0; i < csr.nrows(); i++) {
    if (csr.rowPtr()[i] == csr.rowPtr()[i + 1]) ctr++;
  }

  return ctr;
}

template <typename IndexType, typename ValueType>
IndexType numEmptyCols(const CSR<IndexType, ValueType>& csr) {
  IndexType ctr = 0;

  // to change when CSRToCSC is implemented...
  auto coo = convert::CSRToCOO(csr);
  coo.sortByCol();
  auto csc = convert::COOToCSC(coo);

  for (int i = 0; i < csc.ncols(); i++) {
    if (csc.colPtr()[i] == csc.colPtr()[i + 1]) ctr++;
  }

  return ctr;
}

template <typename IndexType, typename ValueType>
IndexType countDiagonal(const CSR<IndexType, ValueType>& csr) {
  if (csr.nrows() != csr.ncols()) {
    throw std::runtime_error("Matrix is not square");
  }

  IndexType count{0};
  for (IndexType i = 0; i < csr.nrows(); i++) {
    for (IndexType j = csr.rowPtr()[i]; j < csr.rowPtr()[i + 1]; j++) {
      IndexType col{csr.colIdx()[j]};
      ValueType val{csr.vals()[j]};

      if (col == i) count++;
    }
  }
  return count;
}

// graph
template <typename IndexType, typename ValueType>
IndexType averageDegree(const CSR<IndexType, ValueType>& csr,
                        const bool exclude_empty_rows) {
  if (csr.nrows() != csr.ncols()) {
    throw std::runtime_error(
        "Matrix does not represent an adjacency matrix of a graph (not "
        "square)");
  }

  return (csr.nnzs() + countDiagonal(csr)) / csr.nrows();
}

// average degree
// average degree cols
// average degree rows
// minimum degree cols
// minimum degree rows
// elements in diagonal of length n
// elements in block diagonal of length n
// ELL padding

}  // namespace mtx::features