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

    void add_entry(size_t row, size_t col, const double& value) {
        if (row >= num_rows || col >= num_cols) {
            throw std::out_of_range("COO: index out of bounds");
        }
        row_indices.push_back(row);
        col_indices.push_back(col);
        values.push_back(value);
    }

    void clear() {
        row_indices.clear();
        col_indices.clear();
        values.clear();
    }
};

void printCOO(const COO& entries);
void sortCOO(COO& entries);
