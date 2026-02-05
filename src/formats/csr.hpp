#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

#include "concepts.hpp"

namespace mtx {

template <SparseIndex IndexType, SparseValue ValueType>
class CSR {
 public:
  using deleter_t = void (*)(void *) noexcept;

  // 1) Caller retains ownership of externally allocated buffers
  CSR(IndexType *row_ptr, IndexType *col_idx, ValueType *vals, IndexType nrows,
      IndexType ncols, IndexType nnzs)
  noexcept
      : row_ptr_(row_ptr),
        col_idx_(col_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnzs_(nnzs) {}

  // 2) Adopt ownership of externally allocated buffers
  struct adopt_t {};
  static constexpr adopt_t adopt{};

  CSR(adopt_t, IndexType *row_ptr, IndexType *col_idx, ValueType *vals,
      IndexType nrows, IndexType ncols, IndexType nnzs)
  noexcept
      : row_ptr_(row_ptr),
        col_idx_(col_idx),
        vals_(vals),
        nrows_(nrows),
        ncols_(ncols),
        nnzs_(nnzs),
        row_ptr_owner_(row_ptr, csr_deleter),
        col_idx_owner_(col_idx, csr_deleter),
        vals_owner_(vals, csr_deleter) {}

  void print() const;

  IndexType *rowPtr() noexcept { return row_ptr_; }
  IndexType *colIdx() noexcept { return col_idx_; }
  ValueType *vals() noexcept { return vals_; }

  const IndexType *rowPtr() const noexcept { return row_ptr_; }
  const IndexType *colIdx() const noexcept { return col_idx_; }
  const ValueType *vals() const noexcept { return vals_; }

  IndexType nrows() const noexcept { return nrows_; }
  IndexType ncols() const noexcept { return ncols_; }
  IndexType nnzs() const noexcept { return nnzs_; }

  bool ownsData() const noexcept { return row_ptr_owner_ != nullptr; }

  CSR(const CSR &) = delete;
  CSR &operator=(const CSR &) = delete;

  CSR(CSR &&) = default;
  CSR &operator=(CSR &&) = default;

  ~CSR() = default;

 private:
  static void csr_deleter(void *p) noexcept { std::free(p); }

  IndexType nrows_{0};
  IndexType ncols_{0};
  IndexType nnzs_{0};

  IndexType *row_ptr_ = nullptr;
  IndexType *col_idx_ = nullptr;
  ValueType *vals_ = nullptr;

  std::unique_ptr<void, deleter_t> row_ptr_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> col_idx_owner_{nullptr, nullptr};
  std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

}  // namespace mtx

#ifdef _HEADER_ONLY
#include "csr.cpp"
#endif
