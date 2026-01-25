#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

namespace mtx {

template <typename ValueType>
class BELL {
 public:
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of externally allocated buffers
  BELL(size_t *col_block_idx, ValueType *vals, size_t block_size,
       size_t ell_cols, size_t nrows, size_t ncols, size_t nnz)
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

  BELL(adopt_t, size_t *col_block_idx, ValueType *vals, size_t block_size,
       size_t ell_cols, size_t nrows, size_t ncols, size_t nnz)
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

  size_t *colBlockIdx() noexcept { return col_block_idx_; }
  ValueType *vals() noexcept { return vals_; }

  const size_t *colBlockIdx() const noexcept { return col_block_idx_; }
  const ValueType *vals() const noexcept { return vals_; }

  size_t blockSize() const noexcept { return block_size_; }
  size_t ellCols() const noexcept { return ell_cols_; }
  size_t nrows() const noexcept { return nrows_; }
  size_t ncols() const noexcept { return ncols_; }

  bool ownsData() const noexcept { return col_block_idx_owner_ != nullptr; }

  BELL(const BELL &) = delete;
  BELL &operator=(const BELL &) = delete;

  BELL(BELL &&) = default;
  BELL &operator=(BELL &&) = default;

  ~BELL() = default;

 private:
  static void bell_deleter(void *p) noexcept { std::free(p); }

  size_t block_size_ = 0;
  size_t ell_cols_ = 0;
  size_t nrows_ = 0;
  size_t ncols_ = 0;

  size_t *col_block_idx_ = nullptr;
  ValueType *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> col_block_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

}  // namespace mtx

#ifdef _HEADER_ONLY
#include "bell.cpp"
#endif
