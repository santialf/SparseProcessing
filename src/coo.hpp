#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

namespace mtx {

template <typename Value> class COO {
public:
  enum class Order { Unsorted, RowMajor, ColMajor };
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of row/col/val
  COO(size_t *row_idx, size_t *col_idx, Value *vals, size_t nrows, size_t ncols,
      size_t nnz, Order order = Order::Unsorted)
  noexcept
      : row_idx_(row_idx), col_idx_(col_idx), vals_(vals), nrows_(nrows),
        ncols_(ncols), nnz_(nnz), order_(order) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  COO(adopt_t, size_t *row_idx, size_t *col_idx, Value *vals, size_t nrows,
      size_t ncols, size_t nnz, Order order = Order::Unsorted)
  noexcept
      : row_idx_(row_idx), col_idx_(col_idx), vals_(vals), nrows_(nrows),
        ncols_(ncols), nnz_(nnz), order_(order),
        row_idx_owner_(row_idx, coo_deleter),
        col_idx_owner_(col_idx, coo_deleter), vals_owner_(vals, coo_deleter) {}

  void sortByRow();
  void sortByCol();
  void print() const;

  bool ownsData() const noexcept { return row_idx_owner_ != nullptr; }
  bool isCooUnsorted() const noexcept { return order_ == Order::Unsorted; }
  bool isCooRowMajor() const noexcept { return order_ == Order::RowMajor; }
  bool isCooColMajor() const noexcept { return order_ == Order::ColMajor; }

  size_t *rowIdx() noexcept { return row_idx_; }
  size_t *colIdx() noexcept { return col_idx_; }
  Value *vals() noexcept { return vals_; }

  const size_t *rowIdx() const noexcept { return row_idx_; }
  const size_t *colIdx() const noexcept { return col_idx_; }
  const Value *vals() const noexcept { return vals_; }

  size_t nrows() const noexcept { return nrows_; }
  size_t ncols() const noexcept { return ncols_; }
  size_t nnz() const noexcept { return nnz_; }

  COO(const COO &) = delete;
  COO &operator=(const COO &) = delete;

  COO(COO &&) = default;
  COO &operator=(COO &&) = default;

  ~COO() = default;

private:
  static void coo_deleter(void *p) noexcept { std::free(p); }

  Order order_ = Order::Unsorted;
  void sort(Order);

  size_t nrows_ = 0;
  size_t ncols_ = 0;
  size_t nnz_ = 0;

  size_t *row_idx_ = nullptr;
  size_t *col_idx_ = nullptr;
  Value *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> row_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> col_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "coo.cpp"
#endif
