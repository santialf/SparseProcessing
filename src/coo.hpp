#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class COO {
public:
    size_t num_rows;
    size_t num_cols;

    std::vector<size_t> row_indices;
    std::vector<size_t> col_indices;
    std::vector<double> values;

    COO(size_t rows_, size_t cols_)
    : num_rows(rows_), num_cols(cols_) {}

    void add_entry(size_t, size_t, const double&);
    void sort();
    void clear();
    void print();
};
