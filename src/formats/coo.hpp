#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

#include "concepts.hpp"

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
class COO {
 public:
  enum class Order { Unsorted, RowMajor, ColMajor };
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of externally allocated buffers
  COO(IndexType *row_idx, IndexType *col_idx, ValueType *vals, IndexType nrows,
      IndexType ncols, IndexType nnzs, Order order = Order::Unsorted)
  noexcept
      : row_idx_(row_idx),
        col_idx_(col_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnzs_(nnzs),
        order_(order) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  COO(adopt_t, IndexType *row_idx, IndexType *col_idx, ValueType *vals,
      IndexType nrows, IndexType ncols, IndexType nnzs,
      Order order = Order::Unsorted)
  noexcept
      : row_idx_(row_idx),
        col_idx_(col_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnzs_(nnzs),
        order_(order),
        row_idx_owner_(row_idx, coo_deleter),
        col_idx_owner_(col_idx, coo_deleter),
        vals_owner_(vals, coo_deleter) {}

  void sortByRow();
  void sortByCol();
  void print() const;

  bool ownsData() const noexcept { return row_idx_owner_ != nullptr; }
  bool isCooUnsorted() const noexcept { return order_ == Order::Unsorted; }
  bool isCooRowMajor() const noexcept { return order_ == Order::RowMajor; }
  bool isCooColMajor() const noexcept { return order_ == Order::ColMajor; }

  IndexType *rowIdx() noexcept { return row_idx_; }
  IndexType *colIdx() noexcept { return col_idx_; }
  ValueType *vals() noexcept { return vals_; }

  const IndexType *rowIdx() const noexcept { return row_idx_; }
  const IndexType *colIdx() const noexcept { return col_idx_; }
  const ValueType *vals() const noexcept { return vals_; }

  IndexType nrows() const noexcept { return nrows_; }
  IndexType ncols() const noexcept { return ncols_; }
  IndexType nnzs() const noexcept { return nnzs_; }

  COO(const COO &) = delete;
  COO &operator=(const COO &) = delete;

  COO(COO &&) = default;
  COO &operator=(COO &&) = default;

  ~COO() = default;

 private:
  static void coo_deleter(void *p) noexcept { std::free(p); }

  Order order_ = Order::Unsorted;
  void sort(Order);

  IndexType nrows_ = 0;
  IndexType ncols_ = 0;
  IndexType nnzs_ = 0;

  IndexType *row_idx_ = nullptr;
  IndexType *col_idx_ = nullptr;
  ValueType *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> row_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> col_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

}  // namespace mtx

#ifdef _HEADER_ONLY
#include "coo.cpp"
#endif
