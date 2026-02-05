#include "coo.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
void COO<IndexType, ValueType>::print() const {
  std::cout << "rows: " << nrows_ << " "
            << "cols: " << ncols_ << " "
            << "nnzs: " << nnzs_ << "\n";

  for (IndexType i = 0; i < nnzs_; i++) {
    std::cout << row_idx_[i] << " " << col_idx_[i] << " " << vals_[i] << "\n";
  }
}

template <SparseIndex IndexType, SparseValue ValueType>
void COO<IndexType, ValueType>::sort(Order order) {
  if (nnzs_ <= 1) return;

  std::vector<IndexType> perm(nnzs_);
  std::iota(perm.begin(), perm.end(), 0);

  auto cmp = [&](IndexType a, IndexType b) {
    if (order == Order::RowMajor) {
      if (row_idx_[a] != row_idx_[b]) return row_idx_[a] < row_idx_[b];
      return col_idx_[a] < col_idx_[b];
    } else {
      if (col_idx_[a] != col_idx_[b]) return col_idx_[a] < col_idx_[b];
      return row_idx_[a] < row_idx_[b];
    }
  };

  std::sort(perm.begin(), perm.end(), cmp);

  auto tmp_row = std::make_unique<IndexType[]>(nnzs_);
  auto tmp_col = std::make_unique<IndexType[]>(nnzs_);
  auto tmp_val = std::make_unique<ValueType[]>(nnzs_);

  for (IndexType i = 0; i < nnzs_; ++i) {
    IndexType j = perm[i];
    tmp_row[i] = row_idx_[j];
    tmp_col[i] = col_idx_[j];
    tmp_val[i] = vals_[j];
  }

  std::memcpy(row_idx_, tmp_row.get(), nnzs_ * sizeof(IndexType));
  std::memcpy(col_idx_, tmp_col.get(), nnzs_ * sizeof(IndexType));
  std::memcpy(vals_, tmp_val.get(), nnzs_ * sizeof(ValueType));

  // Better than memcpy: swap ownership
  /* row_.swap(tmp_row);
  col_.swap(tmp_col);
  val_.swap(tmp_val); */
}

template <SparseIndex IndexType, SparseValue ValueType>
void COO<IndexType, ValueType>::sortByRow() {
  if (order_ == Order::RowMajor) return;
  sort(Order::RowMajor);
  order_ = Order::RowMajor;
}

template <SparseIndex IndexType, SparseValue ValueType>
void COO<IndexType, ValueType>::sortByCol() {
  if (order_ == Order::ColMajor) return;
  sort(Order::ColMajor);
  order_ = Order::ColMajor;
}

}  // namespace mtx