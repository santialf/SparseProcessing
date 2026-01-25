#include <iostream>

#include "csc.hpp"

namespace mtx {

template <typename valueType> void CSC<valueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << " "
            << "nnzs: " << nnz_ << "\n";

  std::cout << "rowIdx: ";
  for (size_t i = 0; i < nnz_; i++) {
    std::cout << row_idx_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "colPtr: ";
  for (size_t i = 0; i < ncols_ + 1; i++) {
    std::cout << col_ptr_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "vals: ";
  for (size_t i = 0; i < nnz_; i++) {
    std::cout << vals_[i] << " ";
  }
  std::cout << "\n";
}

} // namespace mtx