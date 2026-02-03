#include "bell.hpp"

#include <iostream>

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
void BELL<IndexType, ValueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << "\n"
            << "ellCols: " << ell_cols_ << "\n";

  std::cout << "colBlockIdx: ";
  for (IndexType i = 0; i < this->nblocks(); i++) {
    std::cout << col_block_idx_[i] << " ";
  }
  std::cout << "\n";
  std::cout << "vals: ";
  for (IndexType i = 0; i < this->nvals(); i++) {
    std::cout << vals_[i] << " ";
  }
  std::cout << "\n";
}

}  // namespace mtx