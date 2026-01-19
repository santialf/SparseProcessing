#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "coo.hpp"

namespace mtx {

template<typename valueType>
class CSR {
public:
    size_t num_rows;
    size_t num_cols;

    std::vector<size_t> row_ptr;
    std::vector<size_t> col_indices;
    std::vector<valueType> values;

    CSR(size_t rows_, size_t cols_)
    : num_rows(rows_), num_cols(cols_) {}

    void clear();
    void print();
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "csr.cpp"
#endif
