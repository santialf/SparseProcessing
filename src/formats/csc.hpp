#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

#include "concepts.hpp"

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
class CSC {
 public:
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of externally allocated buffers
  CSC(IndexType *row_idx, IndexType *col_ptr, ValueType *vals, IndexType nrows,
      IndexType ncols, IndexType nnz)
  noexcept
      : row_idx_(row_idx),
        col_ptr_(col_ptr),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnz_(nnz) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  CSC(adopt_t, IndexType *row_idx, IndexType *col_ptr, ValueType *vals,
      IndexType nrows, IndexType ncols, IndexType nnz)
  noexcept
      : row_idx_(row_idx),
        col_ptr_(col_ptr),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnz_(nnz),
        row_idx_owner_(row_idx, csc_deleter),
        col_ptr_owner_(col_ptr, csc_deleter),
        vals_owner_(vals, csc_deleter) {}

  void print() const;

  IndexType *rowIdx() noexcept { return row_idx_; }
  IndexType *colPtr() noexcept { return col_ptr_; }
  ValueType *vals() noexcept { return vals_; }

  const IndexType *rowIdx() const noexcept { return row_idx_; }
  const IndexType *colPtr() const noexcept { return col_ptr_; }
  const ValueType *vals() const noexcept { return vals_; }

  IndexType nrows() const noexcept { return nrows_; }
  IndexType ncols() const noexcept { return ncols_; }
  IndexType nnz() const noexcept { return nnz_; }

  bool ownsData() const noexcept { return col_ptr_owner_ != nullptr; }

  CSC(const CSC &) = delete;
  CSC &operator=(const CSC &) = delete;

  CSC(CSC &&) = default;
  CSC &operator=(CSC &&) = default;

  ~CSC() = default;

 private:
  static void csc_deleter(void *p) noexcept { std::free(p); }

  IndexType nrows_ = 0;
  IndexType ncols_ = 0;
  IndexType nnz_ = 0;

  IndexType *row_idx_ = nullptr;
  IndexType *col_ptr_ = nullptr;
  ValueType *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> row_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> col_ptr_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

}  // namespace mtx

#ifdef _HEADER_ONLY
#include "csc.cpp"
#endif
