#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

namespace mtx {

template <typename ValueType>
class CSR {
 public:
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of externally allocated buffers
  CSR(size_t *row_ptr, size_t *col_idx, ValueType *vals, size_t nrows,
      size_t ncols, size_t nnz)
  noexcept
      : row_ptr_(row_ptr),
        col_idx_(col_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnz_(nnz) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  CSR(adopt_t, size_t *row_ptr, size_t *col_idx, ValueType *vals, size_t nrows,
      size_t ncols, size_t nnz)
  noexcept
      : row_ptr_(row_ptr),
        col_idx_(col_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnz_(nnz),
        row_ptr_owner_(row_ptr, csr_deleter),
        col_idx_owner_(col_idx, csr_deleter),
        vals_owner_(vals, csr_deleter) {}

  void print() const;

  size_t *rowPtr() noexcept { return row_ptr_; }
  size_t *colIdx() noexcept { return col_idx_; }
  ValueType *vals() noexcept { return vals_; }

  const size_t *rowPtr() const noexcept { return row_ptr_; }
  const size_t *colIdx() const noexcept { return col_idx_; }
  const ValueType *vals() const noexcept { return vals_; }

  size_t nrows() const noexcept { return nrows_; }
  size_t ncols() const noexcept { return ncols_; }
  size_t nnz() const noexcept { return nnz_; }

  bool ownsData() const noexcept { return row_ptr_owner_ != nullptr; }

  CSR(const CSR &) = delete;
  CSR &operator=(const CSR &) = delete;

  CSR(CSR &&) = default;
  CSR &operator=(CSR &&) = default;

  ~CSR() = default;

 private:
  static void csr_deleter(void *p) noexcept { std::free(p); }

  size_t nrows_ = 0;
  size_t ncols_ = 0;
  size_t nnz_ = 0;

  size_t *row_ptr_ = nullptr;
  size_t *col_idx_ = nullptr;
  ValueType *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> row_ptr_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> col_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

}  // namespace mtx

#ifdef _HEADER_ONLY
#include "csr.cpp"
#endif
