#include "csc.hpp"

#include <iostream>

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
void CSC<IndexType, ValueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << " "
            << "nnzs: " << nnzs_ << "\n";

  std::cout << "rowIdx: ";
  for (IndexType i = 0; i < nnzs_; i++) {
    std::cout << row_idx_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "colPtr: ";
  for (IndexType i = 0; i < ncols_ + 1; i++) {
    std::cout << col_ptr_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "vals: ";
  for (IndexType i = 0; i < nnzs_; i++) {
    std::cout << vals_[i] << " ";
  }
  std::cout << "\n";
}

}  // namespace mtx