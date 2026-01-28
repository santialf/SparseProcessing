#include "bell.hpp"

#include <iostream>

namespace mtx {

template <typename ValueType>
void BELL<ValueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << "\n"
            << "ellCols: " << ell_cols_ << "\n";

  std::cout << "colBlockIdx: ";
  size_t nblocks = ell_cols_ / block_size_ * nrows_ / block_size_;
  for (size_t i = 0; i < nblocks; i++) {
    std::cout << col_block_idx_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "vals: ";
  for (size_t i = 0; i < ell_cols_ * nrows_; i++) {
    std::cout << vals_[i] << " ";
  }
  std::cout << "\n";
}

}  // namespace mtx