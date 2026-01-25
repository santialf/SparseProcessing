#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

namespace mtx {

template <typename Value> class CSC {
public:
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of row/col/val
  CSC(size_t *row_idx, size_t *col_ptr, Value *vals, size_t nrows, size_t ncols,
      size_t nnz)
  noexcept
      : row_idx_(row_idx), col_ptr_(col_ptr), vals_(vals), nrows_(nrows),
        ncols_(ncols), nnz_(nnz) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  CSC(adopt_t, size_t *row_idx, size_t *col_ptr, Value *vals, size_t nrows,
      size_t ncols, size_t nnz)
  noexcept
      : row_idx_(row_idx), col_ptr_(col_ptr), vals_(vals), nrows_(nrows),
        ncols_(ncols), nnz_(nnz), row_idx_owner_(row_idx, csc_deleter),
        col_ptr_owner_(col_ptr, csc_deleter), vals_owner_(vals, csc_deleter) {}

  void print() const;

  size_t *rowIdx() noexcept { return row_idx_; }
  size_t *colPtr() noexcept { return col_ptr_; }
  Value *vals() noexcept { return vals_; }

  const size_t *rowIdx() const noexcept { return row_idx_; }
  const size_t *colPtr() const noexcept { return col_ptr_; }
  const Value *vals() const noexcept { return vals_; }

  size_t nrows() const noexcept { return nrows_; }
  size_t ncols() const noexcept { return ncols_; }
  size_t nnz() const noexcept { return nnz_; }

  bool ownsData() const noexcept { return col_ptr_owner_ != nullptr; }

  CSC(const CSC &) = delete;
  CSC &operator=(const CSC &) = delete;

  CSC(CSC &&) = default;
  CSC &operator=(CSC &&) = default;

  ~CSC() = default;

private:
  static void csc_deleter(void *p) noexcept { std::free(p); }

  size_t nrows_ = 0;
  size_t ncols_ = 0;
  size_t nnz_ = 0;

  size_t *row_idx_ = nullptr;
  size_t *col_ptr_ = nullptr;
  Value *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> row_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> col_ptr_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "csc.cpp"
#endif
