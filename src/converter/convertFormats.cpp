#include "convertFormats.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace mtx::convert {

template <typename IndexType, typename ValueType>
CSR<IndexType, ValueType> COOToCSR(const COO<IndexType, ValueType> &coo) {
  if (!coo.isCooRowMajor()) {
    throw std::invalid_argument("COO must be row-major to convert to CSR");
  }

  auto row_ptr = std::make_unique<IndexType[]>(coo.nrows() + 1);
  std::fill(row_ptr.get(), row_ptr.get() + coo.nrows() + 1, 0);
  auto col_idx = std::make_unique<IndexType[]>(coo.nnz());
  auto vals = std::make_unique<ValueType[]>(coo.nnz());

  for (IndexType i = 0; i < coo.nnz(); i++) {
    col_idx[i] = coo.colIdx()[i];
    vals[i] = coo.vals()[i];
    row_ptr[coo.rowIdx()[i] + 1]++;
  }

  for (IndexType i = 0; i < coo.nrows(); i++) {
    row_ptr[i + 1] += row_ptr[i];
  }

  return CSR<IndexType, ValueType>(
      CSR<IndexType, ValueType>::adopt, row_ptr.release(), col_idx.release(),
      vals.release(), coo.nrows(), coo.ncols(), coo.nnz());
}

template <typename IndexType, typename ValueType>
CSC<IndexType, ValueType> COOToCSC(const COO<IndexType, ValueType> &coo) {
  if (!coo.isCooColMajor()) {
    throw std::invalid_argument("COO must be col-major to convert to CSC");
  }

  auto row_idx = std::make_unique<IndexType[]>(coo.nnz());
  auto col_ptr = std::make_unique<IndexType[]>(coo.ncols() + 1);
  auto vals = std::make_unique<ValueType[]>(coo.nnz());
  std::fill(col_ptr.get(), col_ptr.get() + coo.ncols() + 1, 0);

  for (IndexType i = 0; i < coo.nnz(); i++) {
    row_idx[i] = coo.rowIdx()[i];
    vals[i] = coo.vals()[i];
    col_ptr[coo.colIdx()[i] + 1]++;
  }

  for (IndexType i = 0; i < coo.ncols(); i++) {
    col_ptr[i + 1] += col_ptr[i];
  }

  return CSC<IndexType, ValueType>(
      CSC<IndexType, ValueType>::adopt, row_idx.release(), col_ptr.release(),
      vals.release(), coo.nrows(), coo.ncols(), coo.nnz());
}

template <typename IndexType, typename ValueType>
IndexType findEllCols(const COO<IndexType, ValueType> &coo,
                      const IndexType block_size) {
  IndexType ell_col_blocks = 0, current_block_row = -1;
  std::unordered_set<IndexType> seen_block_cols;

  // Goes through all the non-zero elements
  for (IndexType i = 0; i < coo.nnz(); i++) {
    // Compute block row and block column of the nz
    IndexType block_row = coo.rowIdx()[i] / block_size;
    IndexType block_col = coo.colIdx()[i] / block_size;

    // If nz belongs to a different block row, reset the count
    if (block_row != current_block_row) {
      ell_col_blocks = std::max(ell_col_blocks,
                                static_cast<IndexType>(seen_block_cols.size()));
      seen_block_cols.clear();
      current_block_row = block_row;
    }

    // Add distinct block column to the count
    seen_block_cols.insert(block_col);
  }
  ell_col_blocks =
      std::max(ell_col_blocks, static_cast<IndexType>(seen_block_cols.size()));

  return ell_col_blocks * block_size;
}

template <typename IndexType, typename ValueType>
std::unique_ptr<int[]> findColBlockIdx(const COO<IndexType, ValueType> &coo,
                                       const IndexType block_size,
                                       const IndexType ell_cols,
                                       const IndexType padded_rows) {
  const IndexType nblock_rows = padded_rows / block_size;
  const IndexType nblock_cols = ell_cols / block_size;
  const IndexType nblocks = nblock_rows * nblock_cols;

  auto col_block_idx = std::make_unique<int[]>(nblocks);
  std::fill(col_block_idx.get(), col_block_idx.get() + nblocks, -1);

  IndexType current_block_row = -1;
  IndexType slot = 0;
  IndexType prev_k = -1;
  std::vector<IndexType> seen_block_cols;

  // Goes through all the non-zero elements
  for (IndexType i = 0; i < coo.nnz(); i++) {
    // Compute block row and block column of the nz
    IndexType block_row = coo.rowIdx()[i] / block_size;
    IndexType block_col = coo.colIdx()[i] / block_size;

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

template <typename IndexType, typename ValueType>
std::unique_ptr<ValueType[]> findVals(
    const COO<IndexType, ValueType> &coo, const IndexType block_size,
    const IndexType ell_cols, const IndexType padded_rows,
    const std::unique_ptr<int[]> &col_block_idx) {
  const IndexType nblock_cols = ell_cols / block_size;
  auto vals = std::make_unique<ValueType[]>(padded_rows * ell_cols);
  std::fill(vals.get(), vals.get() + padded_rows * ell_cols, 0);

  // Goes through all the non-zero elements
  for (IndexType i = 0; i < coo.nnz(); i++) {
    // Compute block row and block column of the nz
    IndexType block_row = coo.rowIdx()[i] / block_size;
    IndexType block_col = coo.colIdx()[i] / block_size;

    IndexType block_row_offset = block_row * nblock_cols;
    IndexType block_index;

    for (IndexType k = 0; k < nblock_cols; ++k) {
      if (col_block_idx[block_row_offset + k] == block_col) {
        block_index = k;
        break;
      }
    }

    IndexType local_col = coo.colIdx()[i] % block_size;
    IndexType value_index =
        coo.rowIdx()[i] * ell_cols + block_index * block_size + local_col;

    vals[value_index] = coo.vals()[i];
  }

  return vals;
}

template <typename IndexType, typename ValueType>
BELL<IndexType, ValueType> COOToBELL(const COO<IndexType, ValueType> &coo,
                                     const IndexType block_size) {
  if ((block_size <= 0) ||
      (block_size > BELL<IndexType, ValueType>::MAX_BLOCK_SIZE)) {
    throw std::invalid_argument("Block size must be > 0");
  }
  if (!coo.isCooRowMajor()) {
    throw std::invalid_argument("COO must be row-major to convert to BELL");
  }

  const IndexType padded_rows =
      ((coo.nrows() + block_size - 1) / block_size) * block_size;
  const IndexType padded_cols =
      ((coo.ncols() + block_size - 1) / block_size) * block_size;

  const IndexType ell_cols = findEllCols(coo, block_size);
  auto col_block_idx = findColBlockIdx(coo, block_size, ell_cols, padded_rows);
  auto vals = findVals(coo, block_size, ell_cols, padded_rows, col_block_idx);

  return BELL<IndexType, ValueType>(BELL<IndexType, ValueType>::adopt,
                                    col_block_idx.release(), vals.release(),
                                    block_size, ell_cols, padded_rows,
                                    padded_cols, coo.nnz());
}

template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> CSRToCOO(const CSR<IndexType, ValueType> &csr) {
  auto row_idx = std::make_unique<IndexType[]>(csr.nnz());
  auto col_idx = std::make_unique<IndexType[]>(csr.nnz());
  auto vals = std::make_unique<ValueType[]>(csr.nnz());

  IndexType k = 0;
  for (IndexType i = 0; i < csr.nrows(); i++) {
    for (IndexType j = csr.rowPtr()[i]; j < csr.rowPtr()[i + 1]; j++) {
      row_idx[k] = i;
      col_idx[k] = csr.colIdx()[j];
      vals[k] = csr.vals()[j];
      k++;
    }
  }

  return COO<IndexType, ValueType>(
      COO<IndexType, ValueType>::adopt, row_idx.release(), col_idx.release(),
      vals.release(), csr.nrows(), csr.ncols(), csr.nnz(),
      COO<IndexType, ValueType>::Order::RowMajor);
}

template <typename IndexType, typename ValueType>
COO<IndexType, ValueType> CSCToCOO(const CSC<IndexType, ValueType> &csc) {
  auto row_idx = std::make_unique<IndexType[]>(csc.nnz());
  auto col_idx = std::make_unique<IndexType[]>(csc.nnz());
  auto vals = std::make_unique<ValueType[]>(csc.nnz());

  IndexType k = 0;
  for (IndexType i = 0; i < csc.ncols(); i++) {
    for (IndexType j = csc.colPtr()[i]; j < csc.colPtr()[i + 1]; j++) {
      col_idx[k] = i;
      row_idx[k] = csc.rowIdx()[j];
      vals[k] = csc.vals()[j];
      k++;
    }
  }

  return COO<IndexType, ValueType>(
      COO<IndexType, ValueType>::adopt, row_idx.release(), col_idx.release(),
      vals.release(), csc.nrows(), csc.ncols(), csc.nnz(),
      COO<IndexType, ValueType>::Order::ColMajor);
}

}  // namespace mtx::convert