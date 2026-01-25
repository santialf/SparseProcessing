#include "coo.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace mtx {

template <typename valueType>
void COO<valueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << " "
            << "nnzs: " << nnz_ << "\n";

  for (size_t i = 0; i < nnz_; i++) {
    std::cout << row_idx_[i] << " " << col_idx_[i] << " " << vals_[i] << "\n";
  }
}

template <typename valueType>
void COO<valueType>::sort(Order order) {
  if (nnz_ <= 1) return;

  std::vector<size_t> perm(nnz_);
  std::iota(perm.begin(), perm.end(), 0);

  auto cmp = [&](size_t a, size_t b) {
    if (order == Order::RowMajor) {
      if (row_idx_[a] != row_idx_[b]) return row_idx_[a] < row_idx_[b];
      return col_idx_[a] < col_idx_[b];
    } else {
      if (col_idx_[a] != col_idx_[b]) return col_idx_[a] < col_idx_[b];
      return row_idx_[a] < row_idx_[b];
    }
  };

  std::sort(perm.begin(), perm.end(), cmp);

  auto tmp_row = std::make_unique<size_t[]>(nnz_);
  auto tmp_col = std::make_unique<size_t[]>(nnz_);
  auto tmp_val = std::make_unique<valueType[]>(nnz_);

  for (size_t i = 0; i < nnz_; ++i) {
    size_t j = perm[i];
    tmp_row[i] = row_idx_[j];
    tmp_col[i] = col_idx_[j];
    tmp_val[i] = vals_[j];
  }

  std::memcpy(row_idx_, tmp_row.get(), nnz_ * sizeof(size_t));
  std::memcpy(col_idx_, tmp_col.get(), nnz_ * sizeof(size_t));
  std::memcpy(vals_, tmp_val.get(), nnz_ * sizeof(valueType));

  // Better than memcpy: swap ownership
  /* row_.swap(tmp_row);
  col_.swap(tmp_col);
  val_.swap(tmp_val); */
}

template <typename valueType>
void COO<valueType>::sortByRow() {
  if (order_ == Order::RowMajor) return;
  sort(Order::RowMajor);
  order_ = Order::RowMajor;
}

template <typename valueType>
void COO<valueType>::sortByCol() {
  if (order_ == Order::ColMajor) return;
  sort(Order::ColMajor);
  order_ = Order::ColMajor;
}

}  // namespace mtx