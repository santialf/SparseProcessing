#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "coo.hpp"

namespace mtx {

template<typename valueType>
class CSR : public sparseFormat {
public:
    std::vector<size_t> row_ptr;
    std::vector<size_t> col_indices;
    std::vector<valueType> values;

    CSR(size_t rows, size_t cols)
    : sparseFormat(rows, cols) {}

    size_t getNnz() const override {
        return values.size();
    }
    void print();
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "csr.cpp"
#endif
