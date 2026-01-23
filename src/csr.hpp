#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "coo.hpp"

namespace mtx {

template<typename Value>
class CSR {
public:
    using deleter_t = void(*)(void*) noexcept;

    // 1) Caller retains ownership of row/col/val
    CSR(size_t* rowPtr, size_t* colIdx, Value* vals,
        size_t nrows, size_t ncols, size_t nnz) noexcept
        : rowPtr_(rowPtr), colIdx_(colIdx), vals_(vals),
          nrows_(nrows), ncols_(ncols), nnz_(nnz)
    {}

    // 3) Adopt ownership of externally allocated buffers
    struct adopt_t {};
    static constexpr adopt_t adopt{};

    CSR(adopt_t,
        size_t* rowPtr, size_t* colIdx, Value* vals,
        size_t nrows, size_t ncols, size_t nnz) noexcept
        : rowPtr_(rowPtr), colIdx_(colIdx), vals_(vals),
          nrows_(nrows), ncols_(ncols), nnz_(nnz),
          rowPtr_owner_(rowPtr, csr_deleter),
          colIdx_owner_(colIdx, csr_deleter),
          vals_owner_(vals, csr_deleter)
    {}

    void print();

    size_t* rowPtr() noexcept { return rowPtr_; }
    size_t* colIdx() noexcept { return colIdx_; }
    Value* vals() noexcept { return vals_; }

    const size_t* rowPtr() const noexcept { return rowPtr_; }
    const size_t* colIdx() const noexcept { return colIdx_; }
    const Value* vals() const noexcept { return vals_; }

    size_t nrows() const noexcept { return nrows_; }
    size_t ncols() const noexcept { return ncols_; }
    size_t nnz()   const noexcept { return nnz_; }

    bool owns_data() const noexcept { return rowPtr_owner_ != nullptr; }

    CSR(const CSR&) = delete;
    CSR& operator=(const CSR&) = delete;

    CSR(CSR&&) = default;
    CSR& operator=(CSR&&) = default;

    ~CSR() = default;

private:
    static void csr_deleter(void* p) noexcept { std::free(p); }

    size_t nrows_ = 0;
    size_t ncols_ = 0;
    size_t nnz_   = 0;

    size_t* rowPtr_ = nullptr;
    size_t* colIdx_ = nullptr;
    Value* vals_ = nullptr;

    std::unique_ptr<void, deleter_t> rowPtr_owner_{nullptr, nullptr};
    std::unique_ptr<void, deleter_t> colIdx_owner_{nullptr, nullptr};
    std::unique_ptr<void, deleter_t> vals_owner_{nullptr, nullptr};
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "csr.cpp"
#endif
