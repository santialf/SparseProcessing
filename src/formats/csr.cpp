#include "csr.hpp"

#include <iostream>

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
void CSR<IndexType, ValueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << " "
            << "nnzs: " << nnzs_ << "\n";

  std::cout << "rowPtr: ";
  for (IndexType i = 0; i < nrows_ + 1; i++) {
    std::cout << row_ptr_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "colIdx: ";
  for (IndexType i = 0; i < nnzs_; i++) {
    std::cout << col_idx_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "vals: ";
  for (IndexType i = 0; i < nnzs_; i++) {
    std::cout << vals_[i] << " ";
  }
  std::cout << "\n";
}

}  // namespace mtx