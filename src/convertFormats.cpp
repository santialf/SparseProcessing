#include "convertFormats.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace mtx::convert {

template <typename ValueType>
CSR<ValueType> COOToCSR(const COO<ValueType> &coo) {
  if (!coo.isCooRowMajor()) {
    throw std::invalid_argument("COO must be row-major to convert to CSR");
  }

  auto row_ptr = std::make_unique<size_t[]>(coo.nrows() + 1);
  std::fill(row_ptr.get(), row_ptr.get() + coo.nrows() + 1, 0);
  auto col_idx = std::make_unique<size_t[]>(coo.nnz());
  auto vals = std::make_unique<ValueType[]>(coo.nnz());

  for (size_t i = 0; i < coo.nnz(); i++) {
    col_idx[i] = coo.colIdx()[i];
    vals[i] = coo.vals()[i];
    row_ptr[coo.rowIdx()[i] + 1]++;
  }

  for (size_t i = 0; i < coo.nrows(); i++) {
    row_ptr[i + 1] += row_ptr[i];
  }

  return CSR<ValueType>(CSR<ValueType>::adopt, row_ptr.release(),
                        col_idx.release(), vals.release(), coo.nrows(),
                        coo.ncols(), coo.nnz());
}

template <typename ValueType>
CSC<ValueType> COOToCSC(const COO<ValueType> &coo) {
  if (!coo.isCooColMajor()) {
    throw std::invalid_argument("COO must be col-major to convert to CSC");
  }

  auto row_idx = std::make_unique<size_t[]>(coo.nnz());
  auto col_ptr = std::make_unique<size_t[]>(coo.ncols() + 1);
  auto vals = std::make_unique<ValueType[]>(coo.nnz());
  std::fill(col_ptr.get(), col_ptr.get() + coo.ncols() + 1, 0);

  for (size_t i = 0; i < coo.nnz(); i++) {
    row_idx[i] = coo.rowIdx()[i];
    vals[i] = coo.vals()[i];
    col_ptr[coo.colIdx()[i] + 1]++;
  }

  for (size_t i = 0; i < coo.ncols(); i++) {
    col_ptr[i + 1] += col_ptr[i];
  }

  return CSC<ValueType>(CSC<ValueType>::adopt, row_idx.release(),
                        col_ptr.release(), vals.release(), coo.nrows(),
                        coo.ncols(), coo.nnz());
}

template <typename ValueType>
size_t findEllCols(const COO<ValueType> &coo, size_t block_size) {
  size_t ell_col_blocks = 0, current_block_row = -1;
  std::unordered_set<size_t> seen_block_cols;

  // Goes through all the non-zero elements
  for (size_t i = 0; i < coo.nnz(); i++) {
    // Compute block row and block column of the nz
    size_t block_row = coo.rowIdx()[i] / block_size;
    size_t block_col = coo.colIdx()[i] / block_size;

    // If nz belongs to a different block row, reset the count
    if (block_row != current_block_row) {
      if (current_block_row != static_cast<size_t>(-1)) {
        ell_col_blocks = std::max(ell_col_blocks, seen_block_cols.size());
      }
      seen_block_cols.clear();
      current_block_row = block_row;
    }

    // Add distinct block column to the count
    seen_block_cols.insert(block_col);
  }

  return ell_col_blocks * block_size;
}

template <typename ValueType>
std::unique_ptr<size_t[]> findColBlockIdx(const COO<ValueType> &coo,
                                          size_t block_size, size_t ell_cols) {
  size_t nblock_rows = ceil(coo.nrows() / block_size);
  size_t nblock_cols = ell_cols / block_size;
  size_t nblocks = nblock_rows * nblock_cols;

  auto col_block_idx = std::make_unique<size_t[]>(nblocks);
  std::fill(col_block_idx.get(), col_block_idx.get() + nblocks, -1);

  size_t current_block_row = -1;
  size_t slot = 0;
  size_t prev_k = -1;
  std::vector<size_t> seen_block_cols;

  // Goes through all the non-zero elements
  for (size_t i = 0; i < coo.nnz(); i++) {
    // Compute block row and block column of the nz
    size_t block_row = coo.rowIdx()[i] / block_size;
    size_t block_col = coo.colIdx()[i] / block_size;

    // If nz belongs to a different block row, reset the count
    if (block_row != current_block_row) {
      std::sort(seen_block_cols.begin(), seen_block_cols.end());
      for (auto k : seen_block_cols) {
        if (k != prev_k) {
          col_block_idx[current_block_row * nblock_cols + slot] = k;
          slot++;
          prev_k = k;
        }
      }

      current_block_row = block_row;
      slot = 0;
      seen_block_cols.clear();
    }

    seen_block_cols.push_back(block_col);
  }

  slot = 0;
  std::sort(seen_block_cols.begin(), seen_block_cols.end());
  for (auto k : seen_block_cols) {
    if (k != prev_k) {
      col_block_idx[current_block_row * nblock_cols + slot] = k;
      slot++;
      prev_k = k;
    }
  }

  return col_block_idx;
}

template <typename ValueType>
std::unique_ptr<ValueType[]> findVals(
    const COO<ValueType> &coo, size_t block_size, size_t ell_cols,
    const std::unique_ptr<size_t[]> &col_block_idx) {
  size_t nblock_cols = ell_cols / block_size;
  auto vals = std::make_unique<ValueType[]>(coo.nrows() * ell_cols);
  std::fill(vals.get(), vals.get() + coo.nrows() * ell_cols, 0);

  // Goes through all the non-zero elements
  for (size_t i = 0; i < coo.nnz(); i++) {
    // Compute block row and block column of the nz
    size_t block_row = coo.rowIdx()[i] / block_size;
    size_t block_col = coo.colIdx()[i] / block_size;

    size_t block_row_offset = block_row * nblock_cols;
    size_t block_index;

    for (size_t k = 0; k < nblock_cols; ++k) {
      if (col_block_idx[block_row_offset + k] == block_col) {
        block_index = k;
        break;
      }
    }

    size_t local_col = coo.colIdx()[i] % block_size;
    size_t value_index =
        coo.rowIdx()[i] * ell_cols + block_index * block_size + local_col;

    vals[value_index] = coo.vals()[i];
  }

  return vals;
}

template <typename ValueType>
BELL<ValueType> COOToBELL(const COO<ValueType> &coo, size_t block_size) {
  if (!coo.isCooRowMajor()) {
    throw std::invalid_argument("COO must be row-major to convert to BELL");
  }

  size_t ell_cols = findEllCols(coo, block_size);
  size_t nblocks = (coo.nrows() * ell_cols) / block_size;

  auto col_block_idx = findColBlockIdx(coo, block_size, ell_cols);
  auto vals = findVals(coo, block_size, ell_cols, col_block_idx);

  return BELL<ValueType>(BELL<ValueType>::adopt, col_block_idx.release(),
                         vals.release(), block_size, ell_cols, coo.nrows(),
                         coo.ncols(), coo.nnz());
}

template <typename ValueType>
COO<ValueType> CSRToCOO(const CSR<ValueType> &csr) {
  auto row_idx = std::make_unique<size_t[]>(csr.nnz());
  auto col_idx = std::make_unique<size_t[]>(csr.nnz());
  auto vals = std::make_unique<ValueType[]>(csr.nnz());

  size_t k = 0;
  for (size_t i = 0; i < csr.nrows(); i++) {
    for (size_t j = csr.rowPtr()[i]; j < csr.rowPtr()[i + 1]; j++) {
      row_idx[k] = i;
      col_idx[k] = csr.colIdx()[j];
      vals[k] = csr.vals()[j];
      k++;
    }
  }

  return COO<ValueType>(COO<ValueType>::adopt, row_idx.release(),
                        col_idx.release(), vals.release(), csr.nrows(),
                        csr.ncols(), csr.nnz(),
                        COO<ValueType>::Order::RowMajor);
}

template <typename ValueType>
COO<ValueType> CSCToCOO(const CSC<ValueType> &csc) {
  auto row_idx = std::make_unique<size_t[]>(csc.nnz());
  auto col_idx = std::make_unique<size_t[]>(csc.nnz());
  auto vals = std::make_unique<ValueType[]>(csc.nnz());

  size_t k = 0;
  for (size_t i = 0; i < csc.ncols(); i++) {
    for (size_t j = csc.colPtr()[i]; j < csc.colPtr()[i + 1]; j++) {
      col_idx[k] = i;
      row_idx[k] = csc.rowIdx()[j];
      vals[k] = csc.vals()[j];
      k++;
    }
  }

  return COO<ValueType>(COO<ValueType>::adopt, row_idx.release(),
                        col_idx.release(), vals.release(), csc.nrows(),
                        csc.ncols(), csc.nnz(),
                        COO<ValueType>::Order::ColMajor);
}

}  // namespace mtx::convert