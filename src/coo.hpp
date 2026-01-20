#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "format.hpp"

namespace mtx {

template<typename valueType>
class COO : public sparseFormat {
private:
    std::vector<size_t> row_indices;
    std::vector<size_t> col_indices;
    std::vector<valueType> values;

public:
    COO(size_t rows, size_t cols)
    : sparseFormat(rows, cols) {}

    size_t getNnz() const override {
        return values.size();
    }
    void add_entry(size_t, size_t, const valueType&);
    void sort();
    void print();

    const std::vector<size_t>& rowsIdx() const { return row_indices; }
    const std::vector<size_t>& colsIdx() const { return col_indices; }
    const std::vector<valueType>& vals() const { return values; }
};

} // namespace mtx

#ifdef _HEADER_ONLY
#include "coo.cpp"
#endif
