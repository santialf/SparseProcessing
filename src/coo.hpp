#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "format.hpp"

namespace mtx {

template<typename Value>
class COO {
public:
    using deleter_t = void(*)(void*) noexcept;

    // 1) Caller retains ownership of row/col/val
    COO(size_t* row, size_t* col, Value* val,
        size_t nrows, size_t ncols, size_t nnz) noexcept
        : row_(row), col_(col), val_(val),
          nrows_(nrows), ncols_(ncols), nnz_(nnz)
    {}

    // 2) Adopt ownership of externally allocated buffers
    struct adopt_t {};
    static constexpr adopt_t adopt{};

    COO(adopt_t,
        size_t* row, size_t* col, Value* val,
        size_t nrows, size_t ncols, size_t nnz) noexcept
        : row_(row), col_(col), val_(val),
          nrows_(nrows), ncols_(ncols), nnz_(nnz),
          row_owner_(row, coo_deleter),
          col_owner_(col, coo_deleter),
          val_owner_(val, coo_deleter)
    {}

    void print();

    size_t* row() noexcept { return row_; }
    size_t* col() noexcept { return col_; }
    Value* val() noexcept { return val_; }

    const size_t* row() const noexcept { return row_; }
    const size_t* col() const noexcept { return col_; }
    const Value* val() const noexcept { return val_; }

    size_t nrows() const noexcept { return nrows_; }
    size_t ncols() const noexcept { return ncols_; }
    size_t nnz()   const noexcept { return nnz_; }

    bool owns_data() const noexcept {
        return row_owner_ != nullptr;
    }

    COO(const COO&) = delete;
    COO& operator=(const COO&) = delete;

    COO(COO&&) = default;
    COO& operator=(COO&&) = default;

    ~COO() = default;

private:
    static void coo_deleter(void* p) noexcept {
        std::free(p);
    }

    size_t* row_ = nullptr;
    size_t* col_ = nullptr;
    Value* val_ = nullptr;

    size_t nrows_ = 0;
    size_t ncols_ = 0;
    size_t nnz_   = 0;

    std::unique_ptr<void, deleter_t> row_owner_{nullptr, nullptr};
    std::unique_ptr<void, deleter_t> col_owner_{nullptr, nullptr};
    std::unique_ptr<void, deleter_t> val_owner_{nullptr, nullptr};
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "coo.cpp"
#endif
