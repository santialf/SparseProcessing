#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

namespace mtx {

template <typename IndexType, typename ValueType>
class BELL {
 public:
  using deleter_t = void (*)(void *) noexcept;
  static constexpr IndexType MAX_BLOCK_SIZE = 64;

  // 1) Caller retains ownership of externally allocated buffers
  BELL(int *col_block_idx, ValueType *vals, IndexType block_size,
       IndexType ell_cols, IndexType nrows, IndexType ncols, IndexType nnz)
  noexcept
      : block_size_(block_size),
        ell_cols_(ell_cols),
        col_block_idx_(col_block_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  BELL(adopt_t, int *col_block_idx, ValueType *vals, IndexType block_size,
       IndexType ell_cols, IndexType nrows, IndexType ncols, IndexType nnz)
  noexcept
      : block_size_(block_size),
        ell_cols_(ell_cols),
        col_block_idx_(col_block_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        col_block_idx_owner_(col_block_idx, bell_deleter),
        vals_owner_(vals, bell_deleter) {}

  void print() const;

  int *colBlockIdx() noexcept { return col_block_idx_; }
  ValueType *vals() noexcept { return vals_; }

  const int *colBlockIdx() const noexcept { return col_block_idx_; }
  const ValueType *vals() const noexcept { return vals_; }

  IndexType blockSize() const noexcept { return block_size_; }
  IndexType ellCols() const noexcept { return ell_cols_; }
  IndexType nrows() const noexcept { return nrows_; }
  IndexType ncols() const noexcept { return ncols_; }
  IndexType nblocks() const {
    return ell_cols_ / block_size_ * nrows_ / block_size_;
  }
  IndexType nvals() const { return ell_cols_ * nrows_; }

  bool ownsData() const noexcept { return col_block_idx_owner_ != nullptr; }

  BELL(const BELL &) = delete;
  BELL &operator=(const BELL &) = delete;

  BELL(BELL &&) = default;
  BELL &operator=(BELL &&) = default;

  ~BELL() = default;

 private:
  static void bell_deleter(void *p) noexcept { std::free(p); }

  IndexType block_size_ = 0;
  IndexType ell_cols_ = 0;
  IndexType nrows_ = 0;
  IndexType ncols_ = 0;

  int *col_block_idx_ = nullptr;
  ValueType *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> col_block_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

}  // namespace mtx

#ifdef _HEADER_ONLY
#include "bell.cpp"
#endif
